#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Kelimenin harf mi kontrolü (Kelime sýnýrý için)
int isLetter(char c) {
    return isalpha((unsigned char)c);
}

// KMP için LPS dizisini oluþturan fonksiyon
void computeLPSArray(const char* pat, int M, int* lps) {
    int len = 0;
    lps[0] = 0;
    int i = 1;

    while (i < M) {
        if (tolower((unsigned char)pat[i]) == tolower((unsigned char)pat[len])) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

// ANA FONKSÝYON: Arama Motoru Çekirdek Fonksiyonu (KMP + Tam Eþleþme)
void searchEngineKMP_FullMatch(const char* txt, const char* pat, int* current_line, int* total_matches) {
    int M = strlen(pat);
    int N = strlen(txt);

    int* lps = (int*)malloc(sizeof(int) * M);
    computeLPSArray(pat, M, lps);

    int i = 0; 
    int j = 0; 

    while (i < N) {
        if (txt[i] == '\n') {
            (*current_line)++;
        }

        if (tolower((unsigned char)pat[j]) == tolower((unsigned char)txt[i])) {
            j++;
            i++;
        }

        if (j == M) {
            // KELÝME SINIRI KONTROLÜ
            int start_index = i - j;
            int is_full_match = 1; 

            if (start_index > 0 && isLetter(txt[start_index - 1])) {
                is_full_match = 0;
            }
            
            if (i < N && isLetter(txt[i])) {
                is_full_match = 0;
            }

            if (is_full_match) {
                printf("- Kelime bulundu! Satir numarasi: %d\n", *current_line);
                (*total_matches)++;
            }

            j = lps[j - 1];
        } else if (i < N && tolower((unsigned char)pat[j]) != tolower((unsigned char)txt[i])) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    
    free(lps);
}

int main() {
    char filename[256];
    char wordToSearch[256];

    // 1. Kullanýcýdan dosya adýný al
    printf("Aratmak istediginiz metnin ismini giriniz (ornegin: test.txt): ");
    scanf("%255s", filename);

    // 2. Kullanýcýdan aranacak kelimeyi al
    printf("Aratmak istediginiz kelime nedir: ");
    scanf("%255s", wordToSearch);

    // 3. Dosyayý aç ve belleðe al (Dosya Okuma simülasyonu)
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Hata: '%s' dosyasi bulunamadi! Dosya adini ve uzantisini dogru yazdiginizdan emin olun.\n", filename);
        return 1;
    }

    // Dosyanýn boyutunu hesapla
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    // Dosya içeriði için bellek ayýr
    char *textChunk = (char *)malloc(fileSize + 1);
    if (textChunk == NULL) {
        printf("Hata: Bellek ayrilamadi!\n");
        fclose(file);
        return 1;
    }

    // Dosyayý oku ve kapat
    fread(textChunk, 1, fileSize, file);
    textChunk[fileSize] = '\0'; // String'in sonuna bitiþ karakteri ekle
    fclose(file);

    // 4. Arama iþlemini baþlat
    int currentLine = 1; // Baþlangýç satýrý
    int totalMatches = 0;

    printf("\nAranan kelime: '%s'\n", wordToSearch);
    printf("----------------------------------\n");
    searchEngineKMP_FullMatch(textChunk, wordToSearch, &currentLine, &totalMatches);
    printf("----------------------------------\n");
    printf("Toplam '%s' kelimesi (tam eslesme) sayisi: %d\n", wordToSearch, totalMatches);

    // Belleði temizle
    free(textChunk);

    return 0;
}
