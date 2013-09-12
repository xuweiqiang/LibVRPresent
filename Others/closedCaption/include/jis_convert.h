#ifndef JIS_CONVERT_H_
#define JIS_CONVERT_H_

int KANJItoSJIS(unsigned char *jis, unsigned char *sjis, int offset);
int KANJItoUTF16(unsigned char *jis, unsigned short *unicode, int offset);
int ALPHANUMERICtoSJIS(unsigned char *jis, unsigned char *sjis, int offset);
int ALPHANUMERICtoUTF16(unsigned char *jis, unsigned short *unicode, int offset);
int KATAKANAtoSJIS(unsigned char *jis, unsigned char *sjis, int offset);
int KATAKANAtoUTF16(unsigned char *jis, unsigned short *unicode, int offset);
int HIRAGANAtoSJIS(unsigned char *jis, unsigned char *sjis, int offset);
int HIRAGANAtoUTF16(unsigned char *jis, unsigned short *unicode, int offset);
int JIS0201KATAKANAtoSJIS(unsigned char *jis, unsigned char *sjis, int offset);
int JIS0201KATAKANAtoUTF16(unsigned char *jis, unsigned short *unicode, int offset);

bool iskanji1(unsigned char c);
bool iskanji2(unsigned char c);

#endif