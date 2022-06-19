#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fields.h"
#include "dllist.h"
#include <stdbool.h>

#define SIZE 1000

struct DataItem {
   char* data;   
   int key;
};

struct DataItem* hashArray[SIZE]; 
struct DataItem* item;
struct DataItem* itemAtInsert;

int hashCode(int key) {
   return key % SIZE;
}

//ara
struct DataItem *search(int key) {
   //hash'i aldım
   int hashIndex = hashCode(key);  
	
   //NULL görene kadar dizide ilerledik 
   while(hashArray[hashIndex] != NULL) {

      //verilen keyin karşılığını bulursa, bulduğu itemı döndürdüm
      if(hashArray[hashIndex]->key == key)
         return hashArray[hashIndex]; 
			
      //sonraki hücreye ilerledim
      ++hashIndex;
		
      //tabloda kaydırdım
      hashIndex %= SIZE;
   }        
   return NULL;        
}

//ekle
void insert(int key,char* data) {

  //bellekte item için yer ayırdım, ve parametre olarak girilen değerleri atadım
  itemAtInsert = (struct DataItem*) malloc(sizeof(struct DataItem));
  itemAtInsert->data = data;  
  itemAtInsert->key = key;

  //hash code unu al 
  int hashIndex = hashCode(key);
  //boş veya silinmiş bir hücreye kadar dizide hareket et (silinen hücre -1 döndürücek fakat projemde gerek olmadığı için yazmadım.)
  while(hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != -1) {
     //sonraki hücreye ilerle
     ++hashIndex;
	
     //tablo kaydır
     hashIndex %= SIZE;
  }
  hashArray[hashIndex] = itemAtInsert;
}

//////////////////////////////////

int main(int argc, char **argv)
{
  IS is_input;
  IS is_dictionary;
  IS is_input2;
  char *lang;
  int comparedWordCounter = 0;
  int successTranslateWord = 0;
  int unsuccessTranslateWord = 0;
  Dllist dllWords, stmp; //kelimeleri koymak için dll
  int wordsInputLenght;
  FILE *file;

  if (argc != 4) { fprintf(stderr, "usage: printwords language filename dictionary\n"); exit(1); }

  is_input = new_inputstruct(argv[2]); //Dosyayı bir inputstruct olarak aç  
  is_dictionary = new_inputstruct(argv[3]);
  dllWords = new_dllist(); //dll olusturdum

  is_input2 = new_inputstruct(argv[2]); //input dosyasını farklı bir değişkene tekrar attım
 
  // tr veya eng nin doğru girip girmediğinin kontrolü
  lang = argv[1];
  if(strcmp(lang,"tr") != 0 && strcmp(lang,"eng") != 0){
    printf("Error! Translation is limited to tr or eng! Enter the correct command!\n");
    exit(1);
  }

  // Hata kontrolü 
  if (is_input == NULL) {
    perror(argv[2]);
    exit(1);
  }

  // Hata kontrolü 
  if (is_dictionary == NULL) {
    perror(argv[3]);
    exit(1);
  }

  int i;
  int j;

  //ingilizceye çevirilecekse
  if(strcmp(lang,"eng") == 0){
      while(get_line(is_input) >= 0) { // 1
        while(get_line(is_dictionary) >= 0) { // 1001
        for (i = 0; i < is_input->NF; i++) { 
          if(strcmp(is_input->fields[i], is_dictionary->fields[2]) == 0){
            insert(i, strdup(is_dictionary->fields[1]));
            successTranslateWord++;
            break;
          }
        }
      }
    }
  }

  //Türkçeye çevirilecekse
  if(strcmp(lang,"tr") == 0){
      while(get_line(is_input) >= 0) { // 1
        while(get_line(is_dictionary) >= 0) { // 1001
        for (i = 0; i < is_input->NF; i++) { 
          if(strcmp(is_input->fields[i], is_dictionary->fields[1]) == 0){
            insert(i, strdup(is_dictionary->fields[2]));
            successTranslateWord++;
            break;
          }
        }
      }
    }
  }
  
  //inputtaki çevirisi yapılamamış kelimeleri listeye ekle
  while(get_line(is_input2) >= 0) {
    for (i = 0; i < is_input2->NF; i++) {
      comparedWordCounter++; //inputtaki toplam kelime sayısı
      item = search(i);

      //aradığımız keyin verisi hashmapte yoksa önce hashmape ekle sonra dll'e ekle
      if(item == NULL) {
        insert(i, is_input2->fields[i]);
        item = search(i);
        dll_append(dllWords, new_jval_s(strdup(item->data)));
      } else {
        item = search(i);
        dll_append(dllWords, new_jval_s(strdup(item->data)));
      }
    }
  }

  stmp = dllWords->flink;
  //dosyayı yazma yetkisi vererek açtım
  file = fopen("//mnt//c/Users//enes//Desktop//G191210080//txt//output.txt","w");
  //listedeki tüm kelimeleri output.txt ye yaz
  while (stmp != dllWords) {
    fputs(stmp->val.s,file);
    fputs(" ",file);
    stmp = stmp->flink;
  }
  fclose(file);
  //dosyayı okuma yetkisi vererek açtım
  file = fopen("//mnt//c/Users//enes//Desktop//G191210080//txt//output.txt","r");
  char word;
  word = fgetc(file); //dosyadaki ilk char'ı aldım
  //dosyanın sonuna gelene kadar tüm karakterleri oku
  printf("\n");
  while(!feof(file)){
    printf("%c",word);
    word = getc(file);
  }
  fclose(file);

  //toplam kelime sayısından başarıyla çevirilienleri çıkararak, başarısız olanları bul
  unsuccessTranslateWord = comparedWordCounter - successTranslateWord;

  printf("\n\n\e[3m---------------ÖZET BİLGİ---------------\e[m\n\n");
  printf("Karşılaştırılan kelime sayısı: %d\n",comparedWordCounter);
  printf("Çevirisi Başarılı Kelime Sayısı: %d\n",successTranslateWord);
  printf("Çevirisi Yapılamamış Kelime Sayısı: %d\n",unsuccessTranslateWord);

  //bellek boşaltma
  jettison_inputstruct(is_input);
  jettison_inputstruct(is_input2);
  jettison_inputstruct(is_dictionary);
  free_dllist(dllWords);
  free(itemAtInsert);
  return 0;
}
