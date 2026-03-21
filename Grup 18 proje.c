#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h> 

// Konsol yazý rengini deðiþtiren yardýmcý fonksiyon
void konsol_rengi(int renk) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, renk);
}

// Büyük küçük harf duyarsýzlýðý için normalizasyon
void metni_kucult(char *metin) {
    for (int i = 0; metin[i]; i++) {
        metin[i] = tolower(metin[i]);
    }
}

int main() {
    char dosya_yolu[100], aranan[50], satir[1024];
    int satir_sayaci = 0, genel_toplam = 0;

    // -- Arayüz ve Giriþ --
    konsol_rengi(11); 
    printf("===========================================\n");
    printf("   METIN ARAMA VE ANALIZ SISTEMI v1.0\n");
    printf("===========================================\n\n");
    
    konsol_rengi(7); 
    printf("[>] Aranacak Dosya Adi (.txt dahil): ");
    scanf("%s", dosya_yolu);
    printf("[>] Aranacak Kelime: ");
    scanf("%s", aranan);

    // --- Dosya Ýþlemleri ---
    FILE *dosya = fopen(dosya_yolu, "r");
    if (dosya == NULL) {
        konsol_rengi(12); 
        printf("\n[!] HATA: Dosya bulunamadi veya acilamadi!\n");
        konsol_rengi(7);
        return 1;
    }

    // Aranan kelimeyi küçük harfe çevir
    char aranan_kucuk[50];
    strcpy(aranan_kucuk, aranan);
    metni_kucult(aranan_kucuk);

    printf("\n--- Analiz Baslatiliyor ---\n\n");

    // --- Satýr Satýr Okuma ve Arama ---
    while (fgets(satir, sizeof(satir), dosya)) {
        satir_sayaci++;
        
        // Satýrý analiz için kopyala ve küçült
        char satir_kopya[1024];
        strcpy(satir_kopya, satir);
        metni_kucult(satir_kopya);

        int satir_ici_sayac = 0;
        char *isaretci = satir_kopya;

        // Satýr içinde kelimeyi ara
        while ((isaretci = strstr(isaretci, aranan_kucuk)) != NULL) {
            satir_ici_sayac++;
            genel_toplam++;
            isaretci += strlen(aranan_kucuk); 
        }

        // Eðer satýrda kelime bulunduysa raporla
        if (satir_ici_sayac > 0) {
            konsol_rengi(10); 
            printf("[+] Satir %d: %d adet bulundu.\n", satir_sayaci, satir_ici_sayac);
            konsol_rengi(7);
        }
    }

    // Raporlama kýsmý
    printf("\n-------------------------------------------\n");
    konsol_rengi(14); 
    printf("ARAMA TAMAMLANDI!\n");
    printf("Toplam bulunan '%s' sayisi: %d\n", aranan, genel_toplam);
    konsol_rengi(7);
    printf("-------------------------------------------\n");

    fclose(dosya);
    
    // Programýn hemen kapanmamasý için
    printf("\nCikis icin bir tusa basin...");
    getchar(); getchar(); 
    
    return 0;
}
