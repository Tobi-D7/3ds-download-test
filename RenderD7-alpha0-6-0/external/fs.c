#include "fs.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void Utils_U8_To_U16(u16 *buf, const u8 *input, size_t bufsize) {
	ssize_t units = utf8_to_utf16(buf, input, bufsize);

	if (units < 0)
		units = 0;

	buf[units] = 0;
}


static Result Screenshot_GenerateScreenshot(const char *path) {
	int x = 0, y = 0;
	size_t size = 0x36;
	Result ret = 0;

	// Get top/bottom framebuffers
	u8 *top_framebuf = gfxGetFramebuffer(GFX_BOTTOM, GFX_BOTTOM, NULL, NULL);
	u8 *bottom_framebuf = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);

	u8 *buf = NULL;
	buf = linearAlloc(size + 576000);
	memset(buf, 0, size + 576000);
	buf[size + 576000] = 0;

	*(u16*)&buf[0x0] = 0x4D42;
	*(u32*)&buf[0x2] = size + 576000;
	*(u32*)&buf[0xA] = size;
	*(u32*)&buf[0xE] = 0x28;
	*(u32*)&buf[0x12] = 400;
	*(u32*)&buf[0x16] = 480;
	*(u32*)&buf[0x1A] = 0x00180001;
	*(u32*)&buf[0x22] = 576000;

	// Generate top left
	u8 *framebuf = bottom_framebuf;

	for (y = 0; y < 240; y++) {
		for (x = 0; x < 800; x++) {
			int si = ((239 - y) + (x * 240)) * 3;
			int di = size + (x + ((479 - y) *800)) * 3;
			buf[di++] = framebuf[si++];
			buf[di++] = framebuf[si++];
			buf[di++] = framebuf[si++];
		}
	}

	// Generate bottom right
	framebuf = top_framebuf;

	for (y = 0; y < 240; y++) {
		for (x = 0; x < 320; x++) {
			int si = ((239 - y) + (x * 240)) * 3;
			int di = size + ((x+40) + ((239 - y) * 400)) * 3;
			buf[di++] = framebuf[si++];
			buf[di++] = framebuf[si++];
			buf[di++] = framebuf[si++];
		}

		// Make adjustments for the smaller width
		for (x = 0; x < 40; x++) {
			int di = size + (x + ((239 - y) * 400)) * 3;
			buf[di++] = 0;
			buf[di++] = 0;
			buf[di++] = 0;
		}

		for (x = 360; x < 400; x++) {
			int di = size + (x + ((239 - y) * 400)) * 3;
			buf[di++] = 0;
			buf[di++] = 0;
			buf[di++] = 0;
		}
	}

	if (R_FAILED(ret = FS_Write(sdmc_archive, path, (u32 *)buf, size + 576000))) {
		//Menu_DisplayError("FS_Write screenshot failed:", ret);
		free(buf);
	}

	linearFree(buf);
	return 0;
}

static void Screenshot_GenerateFilename(int count, char *file_name) {
	time_t t = time(0);
	int day = localtime(&t)->tm_mday;
	int month = localtime(&t)->tm_mon + 1;
	int year = localtime(&t)->tm_year + 1900;
	
	if (!(FS_DirExists(sdmc_archive, "/BCSTM-Player/screenshots/")))
		FS_MakeDir(sdmc_archive, "/BCSTM-Player/screenshots");
		
	sprintf(file_name, "/BCSTM-Player/screenshots/Screenshot_%02d%02d%02d-%i.bmp", year, month, day, count);
}

Result FS_TakeScreenshot(void)
{
	int num = 0;
	static char file_name[256];
	sprintf(file_name, "%s", "screenshot");
	Screenshot_GenerateFilename(num, file_name);

	while (FS_FileExists(sdmc_archive, file_name)) {
		num++;
		Screenshot_GenerateFilename(num, file_name);
	}

	Screenshot_GenerateScreenshot(file_name);
	num++;
	return 0;
}
FS_Archive archive, sdmc_archive, nand_archive;

Result FS_OpenArchive(FS_Archive *archive, FS_ArchiveID archiveID) {
	Result ret = 0;

	if (R_FAILED(ret = FSUSER_OpenArchive(archive, archiveID, fsMakePath(PATH_EMPTY, ""))))
		return ret;

	return 0;
}

Result FS_CloseArchive(FS_Archive archive) {
	Result ret = 0;

	if (R_FAILED(ret = FSUSER_CloseArchive(archive)))
		return ret;

	return 0;
}

Result FS_OpenDir(Handle *handle, FS_Archive archive, const char *path) {
	Result ret = 0;

	u16 path_u16[strlen(path) + 1];
	Utils_U8_To_U16(path_u16, (const u8 *)path, strlen(path) + 1);

	if (R_FAILED(ret = FSUSER_OpenDirectory(handle, archive, fsMakePath(PATH_UTF16, path_u16))))
		return ret;

	return 0;
}

Result FS_OpenFile(Handle *handle, FS_Archive archive, const char *path, u32 flags, u32 attributes) {
	Result ret = 0;

	u16 path_u16[strlen(path) + 1];
	Utils_U8_To_U16(path_u16, (const u8 *)path, strlen(path) + 1);

	if (R_FAILED(ret = FSUSER_OpenFile(handle, archive, fsMakePath(PATH_UTF16, path_u16), flags, attributes)))
		return ret;

	return 0;
}

Result FS_MakeDir(FS_Archive archive, const char *path) {
	Result ret = 0;

	u16 path_u16[strlen(path) + 1];
	Utils_U8_To_U16(path_u16, (const u8 *)path, strlen(path) + 1);

	if (R_FAILED(ret = FSUSER_CreateDirectory(archive, fsMakePath(PATH_UTF16, path_u16), 0)))
		return ret;

	return 0;
}

Result FS_CreateFile(FS_Archive archive, const char *path, u64 size) {
	Result ret = 0;

	u16 path_u16[strlen(path) + 1];
	Utils_U8_To_U16(path_u16, (const u8 *)path, strlen(path) + 1);

	if (R_FAILED(ret = FSUSER_CreateFile(archive, fsMakePath(PATH_UTF16, path_u16), 0, size)))
		return ret;

	return 0;
}

Result FS_RecursiveMakeDir(FS_Archive archive, const char *path) {
	Result ret = 0;
	char buf[256];
	char *p = NULL;
	size_t len;

	snprintf(buf, sizeof(buf), "%s", path);
	len = strlen(buf);

	if (buf[len - 1] == '/')
		buf[len - 1] = 0;

	for (p = buf + 1; *p; p++) {
		if (*p == '/') {
			*p = 0;

			if (!FS_DirExists(archive, buf))
				ret = FS_MakeDir(archive, buf);

			*p = '/';
		}

		if (!FS_DirExists(archive, buf))
			ret = FS_MakeDir(archive, buf);
	}

	return ret;
}

bool FS_FileExists(FS_Archive archive, const char *path) {
	Handle handle;

	u16 path_u16[strlen(path) + 1];
	Utils_U8_To_U16(path_u16, (const u8 *)path, strlen(path) + 1);

	if (R_FAILED(FSUSER_OpenFile(&handle, archive, fsMakePath(PATH_UTF16, path_u16), FS_OPEN_READ, 0)))
		return false;

	if (R_FAILED(FSFILE_Close(handle)))
		return false;

	return true;
}

bool FS_DirExists(FS_Archive archive, const char *path) {
	Handle handle;

	u16 path_u16[strlen(path) + 1];
	Utils_U8_To_U16(path_u16, (const u8 *)path, strlen(path) + 1);

	if (R_FAILED(FSUSER_OpenDirectory(&handle, archive, fsMakePath(PATH_UTF16, path_u16))))
		return false;

	if (R_FAILED(FSDIR_Close(handle)))
		return false;

	return true;
}

Result FS_GetFileSize(FS_Archive archive, const char *path, u64 *size) {
	Result ret = 0;
	Handle handle;

	u16 path_u16[strlen(path) + 1];
	Utils_U8_To_U16(path_u16, (const u8 *)path, strlen(path) + 1);

	if (R_FAILED(ret = FSUSER_OpenFile(&handle, archive, fsMakePath(PATH_UTF16, path_u16), FS_OPEN_READ, 0)))
		return ret;

	if (R_FAILED(ret = FSFILE_GetSize(handle, size))) {
		FSFILE_Close(handle);
		return ret;
	}

	if (R_FAILED(ret = FSFILE_Close(handle)))
		return ret;

	return 0;
}

u64 FS_GetFreeStorage(FS_SystemMediaType media_type) {
	FS_ArchiveResource resource = {0};

	if (R_SUCCEEDED(FSUSER_GetArchiveResource(&resource, media_type)))
		return (((u64)resource.freeClusters * (u64)resource.clusterSize));

	return 0;
}

u64 FS_GetTotalStorage(FS_SystemMediaType media_type) {
	FS_ArchiveResource resource = {0};

	if (R_SUCCEEDED(FSUSER_GetArchiveResource(&resource, media_type)))
		return (((u64)resource.totalClusters * (u64)resource.clusterSize));

	return 0;
}

u64 FS_GetUsedStorage(FS_SystemMediaType media_type) {
	return (FS_GetTotalStorage(media_type) - FS_GetUsedStorage(media_type));
}

Result FS_RemoveFile(FS_Archive archive, const char *path) {
	Result ret = 0;

	u16 path_u16[strlen(path) + 1];
	Utils_U8_To_U16(path_u16, (const u8 *)path, strlen(path) + 1);

	if (R_FAILED(ret = FSUSER_DeleteFile(archive, fsMakePath(PATH_UTF16, path_u16))))
		return ret;

	return 0;
}

Result FS_RemoveDir(FS_Archive archive, const char *path) {
	Result ret = 0;

	u16 path_u16[strlen(path) + 1];
	Utils_U8_To_U16(path_u16, (const u8 *)path, strlen(path) + 1);

	if (R_FAILED(ret = FSUSER_DeleteDirectory(archive, fsMakePath(PATH_UTF16, path_u16))))
		return ret;

	return 0;
}

Result FS_RemoveDirRecursive(FS_Archive archive, const char *path) {
	Result ret = 0;

	u16 path_u16[strlen(path) + 1];
	Utils_U8_To_U16(path_u16, (const u8 *)path, strlen(path) + 1);

	if (R_FAILED(ret = FSUSER_DeleteDirectoryRecursively(archive, fsMakePath(PATH_UTF16, path_u16))))
		return ret;

	return 0;
}

Result FS_RenameFile(FS_Archive archive, const char *old_filename, const char *new_filename) {
	Result ret = 0;

	u16 old_filename_u16[strlen(old_filename) + 1];
	Utils_U8_To_U16(old_filename_u16, (const u8 *)old_filename, strlen(old_filename) + 1);

	u16 new_filename_u16[strlen(new_filename) + 1];
	Utils_U8_To_U16(new_filename_u16, (const u8 *)new_filename, strlen(new_filename) + 1);

	if (R_FAILED(ret = FSUSER_RenameFile(archive, fsMakePath(PATH_UTF16, old_filename_u16), archive, fsMakePath(PATH_UTF16, new_filename_u16))))
		return ret;

	return 0;
}

Result FS_RenameDir(FS_Archive archive, const char *old_dirname, const char *new_dirname) {
	Result ret = 0;

	u16 old_dirname_u16[strlen(old_dirname) + 1];
	Utils_U8_To_U16(old_dirname_u16, (const u8 *)old_dirname, strlen(old_dirname) + 1);

	u16 new_dirname_u16[strlen(new_dirname) + 1];
	Utils_U8_To_U16(new_dirname_u16, (const u8 *)new_dirname, strlen(new_dirname) + 1);

	if (R_FAILED(ret = FSUSER_RenameDirectory(archive, fsMakePath(PATH_UTF16, old_dirname_u16), archive, fsMakePath(PATH_UTF16, new_dirname_u16))))
		return ret;

	return 0;
}

Result FS_Read(FS_Archive archive, const char *path, u64 size, void *buf) {
	Result ret = 0;
	Handle handle;

	u32 bytes_read = 0;

	if (R_FAILED(ret = FS_OpenFile(&handle, archive, path, FS_OPEN_READ, 0)))
		return ret;

	if (R_FAILED(ret = FSFILE_Read(handle, &bytes_read, 0, buf, size))) {
		FSFILE_Close(handle);
		return ret;
	}

	if (R_FAILED(ret = FSFILE_Close(handle)))
		return ret;

	return 0;
}

Result FS_Write(FS_Archive archive, const char *path, const void *buf, u32 size) {
	Result ret = 0;
	Handle handle;
	u32 bytes_written = 0;

	if (FS_FileExists(archive, path))
		FS_RemoveFile(archive, path);

	u16 path_u16[strlen(path) + 1];
	Utils_U8_To_U16(path_u16, (const u8 *)path, strlen(path) + 1);

	if (R_FAILED(ret = FSUSER_CreateFile(archive, fsMakePath(PATH_UTF16, path_u16), 0, size)))
		return ret;

	if (R_FAILED(ret = FSUSER_OpenFile(&handle, archive, fsMakePath(PATH_UTF16, path_u16), FS_OPEN_WRITE, 0)))
		return ret;

	if (R_FAILED(ret = FSFILE_Write(handle, &bytes_written, 0, buf, size, FS_WRITE_FLUSH))) {
		FSFILE_Close(handle);
		return ret;
	}

	if (R_FAILED(ret = FSFILE_Close(handle)))
		return ret;

	return 0;
}

char *FS_GetFileTimestamp(const char *path) {
	static char timeStr[60];
	u64 mtime = 0;

	if (R_SUCCEEDED(archive_getmtime(path, &mtime))) {
		time_t mt = mtime;
		struct tm *timeStruct = gmtime(&mt);

		int hours = timeStruct->tm_hour;
		int minutes = timeStruct->tm_min;

		int day = timeStruct->tm_mday;
		int month = timeStruct->tm_mon + 1; // January being 0
		int year = timeStruct->tm_year + 1900;

		snprintf(timeStr, 60, "%d/%d/%d %2i:%02i", year, month, day, hours, minutes);
	}
	else
		return NULL;

	return timeStr;
}