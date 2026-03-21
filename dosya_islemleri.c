#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dosya_islemleri.h"

// Metnin tamamını küçük harfe çevirir
void metni_kucult(char *metin) {
    for (int i = 0; metin[i]; i++) {
        metin[i] = tolower(metin[i]);
    }
}

// Dosya işlemlerini metin dosyasına raporlama fonksiyonu
void dosyada_kelime_ara(const char *dosya_yolu, const char *aranan_kelime) {
    char satir[2048];
    int satir_sayaci = 0;
    int genel_toplam = 0;

    // Okunacak metin dosyasını açıyoruz
    FILE *dosya = fopen(dosya_yolu, "r");
    if (dosya == NULL) {
        printf("\n[!] HATA: '%s' dosyasi bulunamadi veya acilamadi!\n", dosya_yolu);
        return;
    }

    // Raporun kaydedileceği yeni metin dosyasını oluşturuyoruz
    FILE *rapor_dosyasi = fopen("arama_raporu.txt", "w");
    if (rapor_dosyasi == NULL) {
        printf("\n[!] HATA: Rapor dosyasi olusturulamadi!\n");
        fclose(dosya);
        return;
    }

    // Aramanın case insensitive olması için kelimeyi küçültüyoruz
    char aranan_kucuk[256];
    strcpy(aranan_kucuk, aranan_kelime);
    metni_kucult(aranan_kucuk);

    // Hem ekrana bilgi veriyoruz hem de rapor dosyasının başlığını yazdırıyoruz
    printf("\n--- '%s' Kelimesi Icin Arama Sonuclari ---\n\n", aranan_kelime);
    fprintf(rapor_dosyasi, "--- '%s' Kelimesi Icin Arama Raporu ---\n", aranan_kelime);
    fprintf(rapor_dosyasi, "Taranan Dosya: %s\n\n", dosya_yolu);

    // Dosyayı sonuna gelene kadar satır satır okuyoruz
    while (fgets(satir, sizeof(satir), dosya)) {
        satir_sayaci++;
        
        char satir_kopya[2048];
        strcpy(satir_kopya, satir);
        metni_kucult(satir_kopya);

        int satir_ici_sayac = 0;
        char *isaretci = satir_kopya;

        // O anki satırın kopyası içinde kelimeyi arıyoruz
        while ((isaretci = strstr(isaretci, aranan_kucuk)) != NULL) {
            satir_ici_sayac++;
            genel_toplam++;
            isaretci += strlen(aranan_kucuk); 
        }

        // Kelime bulunduysa, satır numarasını hem konsola hem de rapor dosyasına yazdırıyoruz
        if (satir_ici_sayac > 0) {
            printf("- Satir %d: %d adet bulundu.\n", satir_sayaci, satir_ici_sayac);
            fprintf(rapor_dosyasi, "- Satir %d: %d adet bulundu.\n", satir_sayaci, satir_ici_sayac);
        }
    }

    // Dosyaları kapatıyoruz
    fclose(dosya);

    // Tüm dosya tarandıktan sonra sonuçları rapor dosyasına yazdırıp dosyayı kapatıyoruz
    fprintf(rapor_dosyasi, "\n-------------------------------------------\n");
    fprintf(rapor_dosyasi, "ARAMA TAMAMLANDI!\n");
    fprintf(rapor_dosyasi, "Toplam '%s' kelimesi tekrari: %d\n", aranan_kelime, genel_toplam);
    fprintf(rapor_dosyasi, "-------------------------------------------\n");
    
    fclose(rapor_dosyasi);

    // Kullanıcıya işlemin bittiğini ve raporun nerede olduğunu haber veriyoruz
    printf("\n-------------------------------------------\n");
    printf("ARAMA TAMAMLANDI!\n");
    printf("Toplam '%s' kelimesi tekrari: %d\n", aranan_kelime, genel_toplam);
    printf("-------------------------------------------\n");
    printf("[+] Detayli sonuclar 'arama_raporu.txt' dosyasina basariyla kaydedildi!\n");
}