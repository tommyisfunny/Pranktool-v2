#include <zipWrapper.h>

UNZIP zip;

fs::FS *filesystem;

static File myfile;
void * myOpen(const char *filename, int32_t *size) {
  myfile = filesystem->open(filename);
  *size = myfile.size();
  return (void *)&myfile;
}
void myClose(void *p) {
  ZIPFILE *pzf = (ZIPFILE *)p;
  File *f = (File *)pzf->fHandle;
  if (f) f->close();
}

int32_t myRead(void *p, uint8_t *buffer, int32_t length) {
  ZIPFILE *pzf = (ZIPFILE *)p;
  File *f = (File *)pzf->fHandle;
  return f->read(buffer, length);
}

int32_t mySeek(void *p, int32_t position, int iType) {
  ZIPFILE *pzf = (ZIPFILE *)p;
  File *f = (File *)pzf->fHandle;
  if (iType == SEEK_SET)
    return f->seek(position);
  else if (iType == SEEK_END) {
    return f->seek(position + pzf->iSize); 
  } else { // SEEK_CUR
    long l = f->position();
    return f->seek(l + position);
  }
}


void testZip(){
  char szComment[256], szName[256];
  unz_file_info fi;
  int rc;
  rc = zip.openZIP("/test/test.zip", myOpen, myClose, myRead, mySeek);

  if(rc != UNZ_OK){
    debugOut("Failed to open zip file: ");
    debugOut((int)rc);
    debugOutln(" (error code)");
    return;
  } 
  debugOutln("Opened zip file successfully.");

  rc = zip.gotoFirstFile();
  if(rc != UNZ_OK){
    debugOut("Failed to goto first file in zip: ");
    debugOut((int)rc);
    debugOutln(" (error code)");
    return;
  }
  debugOutln("Goto first file in zip successfully.");

  rc = zip.getFileInfo(&fi, szName, sizeof(szName), NULL, 0, szComment, sizeof(szComment));
  if(rc != UNZ_OK){
    debugOut("Failed to get file info: ");
    debugOut((int)rc);
    debugOutln(" (error code)");
    return;
  }
  debugOut("File in zip: ");
  debugOut(szName);
  debugOut("\nSize: ");
  debugOut(fi.uncompressed_size);
  debugOut("\ncompressed size: ");
  debugOut(fi.compressed_size);
  debugOut("\n");

  rc = zip.openCurrentFile();
  if(rc != UNZ_OK){
    debugOut("Failed to open current file in zip: ");
    debugOut((int)rc);
    debugOutln(" (error code)");
    return;
  }
  debugOutln("Opened current file in zip successfully.");

  debugOutln("Reading file: ");
  uint8_t buffer[256];
  int bytesRead;
  while((bytesRead = zip.readCurrentFile(buffer, sizeof(buffer))) > 0){
    for(int i = 0; i < bytesRead; i++){
      char sz[2];
      sz[0] = (char)buffer[i];
      sz[1] = '\0';
      Serial.print(sz);
    }
  }

  zip.closeZIP();
  debugOutln("Closed zip file successfully.");
}

int unzipFile(fs::FS *fs, const char *zipFileName, const char *filename, const char *destDir, bool deleteZip) {
    filesystem = fs;
    int rc;
    rc = zip.openZIP(zipFileName, myOpen, myClose, myRead, mySeek);
    if (rc != UNZ_OK) {
        debugOut("Failed to open zip file: ");
        debugOut((int)rc);
        debugOutln(" (error code)");
        return rc;
    }
    debugOutln("Opened zip file successfully.");
    rc = zip.locateFile(filename);
    if (rc != UNZ_OK) {
        debugOut("Failed to locate file in zip: ");
        debugOut((int)rc);
        debugOutln(" (error code)");
        return rc;
    }
    debugOutln("Located file in zip successfully.");
    char szComment[256], szName[256];
    unz_file_info fi;
    rc = zip.getFileInfo(&fi, szName, sizeof(szName), NULL, 0, szComment, sizeof(szComment));
    if (rc != UNZ_OK) {
        debugOut("Failed to get file info: ");
        debugOut((int)rc);
        debugOutln(" (error code)");
        return rc;
    }
    debugOut("File in zip: ");
    debugOut(szName);
    debugOut("\nSize: ");
    debugOut(fi.uncompressed_size);
    debugOut("\ncompressed size: ");
    debugOut(fi.compressed_size);
    debugOut("\n");
    rc = zip.openCurrentFile();
    if (rc != UNZ_OK) {
        debugOut("Failed to open current file in zip: ");
        debugOut((int)rc);
        debugOutln(" (error code)");
        return rc;
    }
    debugOutln("Opened current file in zip successfully.");
    debugOut("opening destination file: ");
    debugOut(String(destDir) + "/" + szName);
    debugOutln("");
    File destFile = filesystem->open(String(destDir) + "/" + szName, FILE_WRITE);
    if (!destFile) {
        debugOut("Failed to open destination file: ");
        debugOut(String(destDir) + "/" + szName);
        debugOutln("");
        zip.closeCurrentFile();
        zip.closeZIP();
        return -1; // Error opening destination file
    }
    debugOutln("Opened destination file successfully.");
    uint8_t buffer[256];
    int bytesRead;
    while ((bytesRead = zip.readCurrentFile(buffer, sizeof(buffer))) > 0) {
        destFile.write(buffer, bytesRead);
    }
    destFile.close();
    debugOutln("Wrote file to destination successfully.");
    zip.closeCurrentFile();
    debugOutln("Closed current file in zip successfully.");
    zip.closeZIP();
    debugOutln("Closed zip file successfully.");
    if (deleteZip) {
        if (filesystem->remove(filename)) {
            debugOut("Deleted zip file: ");
            debugOut(filename);
            debugOutln(" successfully.");
        } else {
            debugOut("Failed to delete zip file: ");
            debugOut(filename);
            debugOutln(" (error code)");
        }
    }
    return 0; // Success
}


int unzipAll(fs::FS *fs, const char *zipFileName, const char *destDir, bool deleteZip) {
    filesystem = fs;
    int rc;
    rc = zip.openZIP(zipFileName, myOpen, myClose, myRead, mySeek);
    if (rc != UNZ_OK) {
        debugOut("Failed to open zip file: ");
        debugOut((int)rc);
        debugOutln(" (error code)");
        return rc;
    }
    debugOutln("Opened zip file successfully.");
    
    rc = zip.gotoFirstFile();
    if (rc != UNZ_OK) {
        debugOut("Failed to goto first file in zip: ");
        debugOut((int)rc);
        debugOutln(" (error code)");
        return rc;
    }
    
    do {
        char szComment[256], szName[256];
        unz_file_info fi;
        rc = zip.getFileInfo(&fi, szName, sizeof(szName), NULL, 0, szComment, sizeof(szComment));
        if (rc != UNZ_OK) {
            debugOut("Failed to get file info: ");
            debugOut((int)rc);
            debugOutln(" (error code)");
            continue; // Skip this file
        }
        
        debugOut("File in zip: ");
        debugOut(szName);
        debugOut("\nSize: ");
        debugOut(fi.uncompressed_size);
        debugOut("\ncompressed size: ");
        debugOut(fi.compressed_size);
        debugOut("\n");
        
        rc = zip.openCurrentFile();
        if (rc != UNZ_OK) {
            debugOut("Failed to open current file in zip: ");
            debugOut((int)rc);
            debugOutln(" (error code)");
            continue; // Skip this file
        }
        
        File destFile = filesystem->open(String(destDir) + "/" + szName, "wb");
        if (!destFile) {
            debugOut("Failed to open destination file: ");
            debugOut(String(destDir) + "/" + szName);
            debugOutln("");
            zip.closeCurrentFile();
            continue; // Skip this file
        }
        
        uint8_t buffer[256];
        int bytesRead;
        while ((bytesRead = zip.readCurrentFile(buffer, sizeof(buffer))) > 0) {
            destFile.write(buffer, bytesRead);
        }
        
        destFile.close();
        
        zip.closeCurrentFile();
        debugOutln("Closed current file in zip successfully.");
        debugOut("Extracted file: ");
        debugOut(szName);
        debugOut(" to ");
        debugOut(String(destDir) + "/" + szName);
        debugOutln(" successfully.");
    } while (zip.gotoNextFile() == UNZ_OK);
    zip.closeZIP();
    debugOutln("Closed zip file successfully.");
    if (deleteZip) {
        if (filesystem->remove(zipFileName)) {
            debugOut("Deleted zip file: ");
            debugOut(zipFileName);
            debugOutln(" successfully.");
        } else {
            debugOut("Failed to delete zip file: ");
            debugOut(zipFileName);
            debugOutln(" (error code)");
        }
    }
    return 0; // Success
}