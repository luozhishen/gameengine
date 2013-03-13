#ifndef __ATLAS_DES64__
#define __ATLAS_DES64__

typedef bool DES_KEY_S[16][48];
typedef char DES_KEY[8];

void DES_GenKey(DES_KEY key);
bool DES_KeyFromString(DES_KEY key, const char* s);
void DES_KeyToString(const DES_KEY key, char* s);
void DES_SetKey(const DES_KEY key, DES_KEY_S keys);
void DES_Encrypt(DES_KEY_S keys, char Out[8], const char In[8]);
void DES_Decrypt(DES_KEY_S keys, char Out[8], const char In[8]);

#endif
