#include <cstring>
#include "download.hpp"
#include "somethingidk.hpp"

namespace d7gfx {
    HTTPClient::HTTPClient() :
            m_finished(true),
            m_cancel(false),
            m_progress(1.0),
            m_progressSize(0.0),
            m_totalSize(0.0),
            m_response(""),
            m_statusCode(-1),
            m_status(d7gfx::HTTPClient::Status::NotStarted) {
                if (!d7gfx::priv::somethingidk::curlInitialized) curl_global_init(CURL_GLOBAL_ALL);
                m_handle = curl_easy_init();
            }

    HTTPClient::~HTTPClient() {
        cancel();
        m_thread.join();
        curl_easy_cleanup(m_handle);
    }

    const std::string HTTPClient::download(const std::string& t_url) {
        if (m_finished) {
            callFunction((d7gfx::HTTPClient::Context) {
                t_url,
                false,
                "",
                false,
                ""
            });

            return getResponse();
        } else {
            return "";
        }
    }

    void HTTPClient::downloadAsync(const std::string& t_url) {
        if (m_finished) {
            m_thread.initialize(std::bind(&d7gfx::HTTPClient::callFunction, this, std::placeholders::_1), (d7gfx::HTTPClient::Context) {
                                    t_url,
                                    false,
                                    "",
                                    false,
                                    ""
                                }, false, false, 8 * 1024);

            m_thread.start();
        }
    }

    void HTTPClient::downloadToFile(const std::string& t_url, const std::string& t_filename) {
        if (m_finished) {
            callFunction((d7gfx::HTTPClient::Context) {
                t_url,
                false,
                "",
                true,
                t_filename
            });
        }
    }

    void HTTPClient::downloadToFileAsync(const std::string& t_url, const std::string& t_filename) {
        if (m_finished) {
            m_thread.initialize(std::bind(&d7gfx::HTTPClient::callFunction, this, std::placeholders::_1), (d7gfx::HTTPClient::Context) {
                                    t_url,
                                    false,
                                    "",
                                    false,
                                    ""
                                }, false, false, 8 * 1024);

            m_thread.start();
        }
    }

    const std::string HTTPClient::post(const std::string& t_url, const std::string& t_parameters) {
        if (m_finished) {
            callFunction((d7gfx::HTTPClient::Context) {
                t_url,
                true,
                t_parameters,
                false,
                ""
            });

            return getResponse();
        } else {
            return "";
        }
    }

    void HTTPClient::postAsync(const std::string& t_url, const std::string& t_parameters) {
        if (m_finished) {
            m_thread.initialize(std::bind(&d7gfx::HTTPClient::callFunction, this, std::placeholders::_1), (d7gfx::HTTPClient::Context) {
                                    t_url,
                                    true,
                                    t_parameters,
                                    false,
                                    ""
                                }, false, false, 8 * 1024);

            m_thread.start();
        }
    }

    void HTTPClient::postToFile(const std::string& t_url, const std::string& t_parameters, const std::string& t_filename) {
        if (m_finished) {
            callFunction((d7gfx::HTTPClient::Context) {
                t_url,
                true,
                t_parameters,
                true,
                t_filename
            });
        }
    }

    void HTTPClient::postToFileAsync(const std::string& t_url, const std::string& t_parameters, const std::string& t_filename) {
        if (m_finished) {
            m_thread.initialize(std::bind(&d7gfx::HTTPClient::callFunction, this, std::placeholders::_1), (d7gfx::HTTPClient::Context) {
                                    t_url,
                                    true,
                                    t_parameters,
                                    false,
                                    ""
                                }, false, false, 8 * 1024);

            m_thread.start();
        }
    }

    void HTTPClient::waitForFinish() {
        if (!m_finished && m_status != d7gfx::HTTPClient::Status::NotStarted) {
            m_thread.join();
        }
    }

    bool HTTPClient::isFinished() {
        return m_finished;
    }

    void HTTPClient::cancel() {
        if (!m_finished && m_status != d7gfx::HTTPClient::Status::NotStarted) {
            m_cancel = true;
            m_thread.join();
        }
    }

    d7gfx::HTTPClient::Status HTTPClient::getStatus() {
        return m_status;
    }

    int HTTPClient::getStatusCode() {
        if (m_finished && m_status != d7gfx::HTTPClient::Status::NotStarted) {
            return m_statusCode;
        } else {
            return -1;
        }
    }

    const std::string HTTPClient::getResponse() {
        if (m_finished && m_status != d7gfx::HTTPClient::Status::NotStarted) {
            return m_response;
        } else {
            return "";
        }
    }

    double HTTPClient::getProgress() {
        return m_progress;
    }

    double HTTPClient::getProgressSize() {
        return m_progressSize;
    }

    double HTTPClient::getTotalSize() {
        return m_totalSize;
    }

    // private methods
    size_t HTTPClient::xferWriteFunction(char* t_ptr, size_t t_size, size_t t_nmemb, void* t_userdata) {
        d7gfx::Thread::sleep();
        return static_cast<d7gfx::HTTPClient*>(t_userdata)->writeFunction(t_ptr, t_size, t_nmemb);
    }

    size_t HTTPClient::writeFunction(char* t_ptr, size_t t_size, size_t t_nmemb) {
        const size_t bsz = t_size * t_nmemb;
        m_response += t_ptr;
        return bsz;
    }

    size_t HTTPClient::xferProgressFunction(void* t_p, double t_total, double t_downloaded, double, double) {
        return static_cast<d7gfx::HTTPClient*>(t_p)->progressFunction(t_total, t_downloaded);
    }

    int HTTPClient::progressFunction(double t_total, double t_downloaded) {
        m_progressSize = t_downloaded;
        m_totalSize = t_total;

        if (t_total > 0.0) {
            m_progress = t_downloaded / t_total;
        } else {
            m_progress = 0.0;
        }

        if (m_cancel) return 1;

        return 0;
    }

    void HTTPClient::callFunction(d7gfx::Parameter t_context) {
        m_finished = false;
        m_cancel = false;
        m_response = "";

        if (d7gfx::priv::somethingidk::socuInitialized) {
            m_status = d7gfx::HTTPClient::Status::Running;
            d7gfx::HTTPClient::Context ctx = t_context.get<d7gfx::HTTPClient::Context>();

            curl_easy_reset(m_handle);
            curl_easy_setopt(m_handle, CURLOPT_URL, ctx.m_url.c_str());
            curl_easy_setopt(m_handle, CURLOPT_USERAGENT, "d7gfxiaLib/0.3.0");
            curl_easy_setopt(m_handle, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(m_handle, CURLOPT_MAXREDIRS, 50L);
            curl_easy_setopt(m_handle, CURLOPT_HTTP_VERSION, (long) CURL_HTTP_VERSION_2TLS);
            curl_easy_setopt(m_handle, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(m_handle, CURLOPT_VERBOSE, 0L);
            curl_easy_setopt(m_handle, CURLOPT_NOPROGRESS, 0L);
            curl_easy_setopt(m_handle, CURLOPT_PROGRESSFUNCTION, d7gfx::HTTPClient::xferProgressFunction);
            curl_easy_setopt(m_handle, CURLOPT_PROGRESSDATA, this);

            if (ctx.m_post) {
                curl_easy_setopt(m_handle, CURLOPT_POSTFIELDS, ctx.m_parameters.c_str());
            }

            if (ctx.m_toFile) {
                FILE* fp = fopen(ctx.m_filename.c_str(), "wb");
                curl_easy_setopt(m_handle, CURLOPT_WRITEFUNCTION, NULL);
                curl_easy_setopt(m_handle, CURLOPT_WRITEDATA, fp);
            } else {
                curl_easy_setopt(m_handle, CURLOPT_WRITEFUNCTION, d7gfx::HTTPClient::xferWriteFunction);
                curl_easy_setopt(m_handle, CURLOPT_WRITEDATA, this);
            }

            CURLcode cres = curl_easy_perform(m_handle);

            if (!m_cancel) {
                switch (cres) {
                    case CURLE_OK:
                        m_status = d7gfx::HTTPClient::Status::OK;
                        break;
                    case CURLE_COULDNT_RESOLVE_HOST:
                        m_status = d7gfx::HTTPClient::Status::ResolveFailed;
                        m_finished = true;
                        return;
                    case CURLE_REMOTE_ACCESS_DENIED:
                        m_status = d7gfx::HTTPClient::Status::AccessDenied;
                        m_finished = true;
                        return;
                    default:
                        m_status = d7gfx::HTTPClient::Status::Failed;
                        m_finished = true;
                        return;
                }
            } else {
                m_status = d7gfx::HTTPClient::Status::Aborted;
                m_finished = true;
                return;
            }

            curl_easy_getinfo(m_handle, CURLINFO_RESPONSE_CODE, &m_statusCode);

            m_finished = true;
        } else {
            m_status = d7gfx::HTTPClient::Status::InternalError;
            m_finished = true;
        }
    }
} /* d7gfx */