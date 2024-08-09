#include "utils.h"

/**
 * Get file size based on given fileContent.
 *
 * @param fileContent provide filePath, will also set fileSize in it.
 * @retval 0    File does not exist.
 * @retval size_t   File size
 *
 */
size_t get_file_size(FileContent *fileContent) {
    FILE *pFile = fopen(fileContent->filePath, "r");
    if (pFile == NULL) {
        printf("Error: %s not found\n", fileContent->filePath);
        return 0;
    }
    fseek(pFile, 0, SEEK_END);
    size_t len = ftell(pFile);
    fclose(pFile);
    fileContent->fileSize = len;
    return len;
}

/**
 * Read File buffer based on given fileContent.
 *
 * @param fileContent   provide filePath, will also set fileBuffer in it.
 * @return  Buffer read from file.
 */
uint8_t *read_file_content(FileContent *fileContent) {
    if (fileContent->fileBuffer == NULL)
        return NULL;
    FILE *pFile = fopen(fileContent->filePath, "rb");
    if (pFile == NULL)
        return NULL;
    fread(fileContent->fileBuffer, fileContent->fileSize, 1, pFile);
    fclose(pFile);
    return fileContent->fileBuffer;
}

/**
 * Write buffer to filePath given by fileContent.
 *
 * @param fileContent   Contains file information.
 * @retval -EBADF   Failed to write file
 *
 */
int write_file_content(pFileContent fileContent) {
    FILE *pFile = fopen(fileContent->filePath, "wb");
    if (pFile == NULL)
        return -EBADF;
    fwrite(fileContent->fileBuffer, fileContent->fileSize, 1, pFile);
    fclose(pFile);
    return 0;
}

/**
 * Parse given config.
 *
 * @param fileContent
 * @param config Config info read from config file
 * @retval -EINVAL Give File not found.
 *
 */
int parse_config(FileContent *fileContent, pConfig config) {
    // Check file size
    if (!get_file_size(fileContent))
        return -EINVAL;

    // Open config file
    FILE *pConfigFile = fopen(fileContent->filePath, "r");
    char key[256];
    uint32_t value = 0;

    // Parse
    while (fscanf(pConfigFile, "%[^=]=%x\n", key, &value) != EOF) {
        if (strcmp(key, "StackBase") == 0) {
            config->StackBase = value;
        } else if (strcmp(key, "StackSize") == 0) {
            config->StackSize = value;
        }
    }

    fclose(pConfigFile);
    return 0;
}
