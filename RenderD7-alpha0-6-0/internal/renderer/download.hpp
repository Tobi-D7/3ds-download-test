#pragma once
#include <3ds.h>
#include <curl/curl.h>
#include <string>
#include "Thread.hpp"

namespace d7gfx {
    class HTTPClient {
    public:
       
        enum class Status {
            NotStarted,    ///< No request was made yet
            Running,       ///< A asynchronous request is running
            OK,            ///< The last request was successfull
            Aborted,       ///< The last request was aborted by the user
            Failed,        ///< The last request failed
            ResolveFailed, ///< The last request failed to resolve the host
            AccessDenied,  ///< The last request failed because the access to the ressource was denied
            InternalError  ///< The last request failed due to an internal error
        };

        HTTPClient();

        ~HTTPClient();

        const std::string download(const std::string& t_url);

        void downloadAsync(const std::string& t_url);

        void downloadToFile(const std::string& t_url, const std::string& t_filename);

        void downloadToFileAsync(const std::string& t_url, const std::string& t_filename);

        const std::string post(const std::string& t_url, const std::string& t_parameters);

        void postAsync(const std::string& t_url, const std::string& t_parameters);

        void postToFile(const std::string& t_url, const std::string& t_parameters, const std::string& t_filename);

        void postToFileAsync(const std::string& t_url, const std::string& t_parameters, const std::string& t_filename);
    
        void waitForFinish();

        bool isFinished();

        void cancel();
        
        d7gfx::HTTPClient::Status getStatus();

        int getStatusCode();

        const std::string getResponse();
       
        double getProgress();

        double getProgressSize();

        double getTotalSize();

    private:
        struct Context {
            /* data */
            const std::string& m_url;
            bool m_post;
            const std::string m_parameters;
            bool m_toFile;
            const std::string& m_filename;
        };

        static size_t xferProgressFunction(void* t_p, double t_total, double t_downloaded, double, double);
        int progressFunction(double t_total, double t_downloaded);

        static size_t xferWriteFunction(char* t_ptr, size_t t_size, size_t t_nmemb, void* t_userdata);
        size_t writeFunction(char* t_ptr, size_t t_size, size_t t_nmemb);

        void callFunction(d7gfx::Parameter t_context);

        /* data */
        std::atomic<bool> m_finished, m_cancel;
        d7gfx::Thread m_thread;
        CURL* m_handle;

        // size/progress
        std::atomic<double> m_progress, m_progressSize, m_totalSize;
        std::string m_response;

        // response status
        std::atomic<int> m_statusCode;
        d7gfx::HTTPClient::Status m_status;
    };
} /* d7gfx */
