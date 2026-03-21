#ifndef DOSYA_ISLEMLERI_H
#define DOSYA_ISLEMLERI_H

// Metni küçük harfe çeviren yardımcı fonksiyon
void metni_kucult(char *metin);

// Dosyayı satır satır okuyup kelime frekansını ve satırları bulan ana fonksiyon
void dosyada_kelime_ara(const char *dosya_yolu, const char *aranan_kelime);

#endif