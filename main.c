// GitHub - datapeice
// MIT license


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <openssl/evp.h>

#define MAX_BUFFER_SIZE 4096

typedef struct {
    const char *filename;
    unsigned char *hash;
    const EVP_MD *hash_type;
} ThreadData;

void compute_hash(FILE *file, unsigned char *hash, const EVP_MD *hash_type) {
    EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(md_ctx, hash_type, NULL);

    unsigned char buffer[MAX_BUFFER_SIZE];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, MAX_BUFFER_SIZE, file)) > 0) {
        EVP_DigestUpdate(md_ctx, buffer, bytes_read);
    }

    unsigned int len;
    EVP_DigestFinal_ex(md_ctx, hash, &len);
    EVP_MD_CTX_free(md_ctx);
}

void *hash_file(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    FILE *file = fopen(data->filename, "rb");

    if (file == NULL) {
        perror("Failed to open file");
        pthread_exit(NULL);
    }

    compute_hash(file, data->hash, data->hash_type);

    fclose(file);
    pthread_exit(NULL);
}

void print_hash(unsigned char *hash, size_t hash_len) {
    for (size_t i = 0; i < hash_len; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: chksm <filename>\n");
        return 1;
    }

    const char *filename = argv[1];

    pthread_t threads[3];
    unsigned char md5_hash[EVP_MAX_MD_SIZE];
    unsigned char sha1_hash[EVP_MAX_MD_SIZE];
    unsigned char sha256_hash[EVP_MAX_MD_SIZE];

    ThreadData data[3] = {
        {filename, md5_hash, EVP_md5()},
        {filename, sha1_hash, EVP_sha1()},
        {filename, sha256_hash, EVP_sha256()}
    };

    for (int i = 0; i < 3; i++) {
        if (pthread_create(&threads[i], NULL, hash_file, &data[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("MD5 hash: ");
    print_hash(md5_hash, EVP_MD_size(EVP_md5()));

    printf("SHA-1 hash: ");
    print_hash(sha1_hash, EVP_MD_size(EVP_sha1()));

    printf("SHA-256 hash: ");
    print_hash(sha256_hash, EVP_MD_size(EVP_sha256()));

    return 0;
}
