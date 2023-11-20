#include <stdio.h>
#include <sodium.h>
#include <curl/curl.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MESSAGE ((const unsigned char *) "Arbitrary data to hash")
#define MESSAGE_LEN 22

int main(void) {

    CURL* curl;
    CURLcode res;

    // Initialize Curl
    res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        fprintf(stderr, "Error initializing Curl: %s\n", curl_easy_strerror(res));
        return 1;
    } 

    curl = curl_easy_init();
    if (curl == NULL) {
        fprintf(stderr, "Error initializing Curl\n");
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, "127.0.0.1:6969");
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "Error performing Curl operation: %s\n", curl_easy_strerror(res));
        return 0;
    }

    // Try to initialize Sodium
    if (sodium_init() < 0) {
        fprintf(stderr, "Error initializing libsodium\n");
        return 1;
    }

    unsigned char hash[crypto_generichash_BYTES]; // The hashed output
    unsigned char key[crypto_generichash_KEYBYTES]; // The key to use

    // Generate a random key
    randombytes_buf(key, sizeof key);

    // Try to hash the message using the key
    if (crypto_generichash(hash, sizeof hash, MESSAGE, MESSAGE_LEN, key, sizeof key) < 0) {
        perror("Error hashing message");
        return 1;
    }

    // Print the hashed message
    printf("Hashed '%s'\n", hash);

    // Cleanup Curl
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}
