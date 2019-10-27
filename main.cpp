#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <algorithm>
#include <sstream>

using namespace std;

int policzLinijki(ifstream &pliczek){
    pliczek.unsetf(ios_base::skipws);
    int i = count(istream_iterator<char>(pliczek),
                istream_iterator<char>(),
                    '\n');
    pliczek.clear();
    pliczek.seekg(0);
    return i+1;
}

void uzupelnijTablice(int &dlugosc, string * tab , ifstream &pliczek){
    for(int i = 0 ; i < dlugosc ; i++)
        getline(pliczek, tab[i]);
    pliczek.clear();
    pliczek.seekg(0);
}

void wypiszTablice(int &dlugosc, string * tab){
    for(int i = 0 ; i < dlugosc ; i++)
        cout << tab[i] << endl;
}

void przygotujTabliceWynikow(string * p, int &iloscLinijekPierwszy ,string * d, int  &iloscLinijekDrugi , int ** tablica){

    for(int i = 0 ; i <= iloscLinijekDrugi ; i++)
        tablica[i][0] = 0;

    for(int i = 0 ; i <= iloscLinijekPierwszy; i++)
        tablica[0][i] = 0;

    for(int i = 1 ; i <= iloscLinijekDrugi ; i++){
        for(int k = 1 ; k <= iloscLinijekPierwszy; k++){
            if(! p[k-1].compare(d[i-1]) ){
               tablica[i][k] = tablica[i-1][k-1] + 1;
            }else{
               tablica[i][k] = max(tablica[i-1][k],tablica[i][k-1]);
            }
        }
    }

    pair<int,int> * lista = new pair<int,int>;

    lista = new pair<int,int>[tablica[iloscLinijekDrugi][iloscLinijekPierwszy]];

    int i = iloscLinijekDrugi, k = iloscLinijekPierwszy;
    int pozycja = tablica[iloscLinijekDrugi][iloscLinijekPierwszy] - 1;

    while(i!=0 && k !=0){
        if(tablica[i][k] == tablica[i-1][k])
            i--;
        else if(tablica[i][k] == tablica[i][k-1])
            k--;
        else{
            lista[pozycja] = make_pair(k-1,i-1);
            pozycja--;
            k--;
            i--;
        }
    }
    /*
    pozycja = 0;
    // wypisanie wspolnego
    cout<< "\nnajdluzszy wspolny podciag tych plikow to: "<<endl;
    for(int i = 0 ; pozycja < tablica[iloscLinijekDrugi][iloscLinijekPierwszy];i++){
        if(lista[pozycja].first == i){
            cout << "pozycja w pierwszym: "<<lista[pozycja].first <<" pozycja w drugim : "<<lista[pozycja].second <<" linia : "<<p[i]<<endl;
            pozycja++;
        }
    }
    pozycja = 0;
    cout << "\ndodatkowo w pierwszym znajuje sie: "<<endl;
    for(int i = 0 ; pozycja < tablica[iloscLinijekDrugi][iloscLinijekPierwszy];i++){
        if(lista[pozycja].first == i){
            pozycja++;
        }else{
             cout << "pozycja w pierwszym: "<< i << " linia : " << p[i]<<endl;
        }
    }
    pozycja = 0;
    cout << "\na w drugim: "<<endl;
    for(int i = 0 ; pozycja < tablica[iloscLinijekDrugi][iloscLinijekPierwszy] ;i++){
        if(lista[pozycja].second == i){
            pozycja++;
        }else{
             cout << "pozycja w drugim: "<< i << " linia : " << d[i]<<endl;
        }
    }
    */

    cout << "\n diff: \n"<<endl;
    pair<int,int> * pomocnicza = lista;
    k = i = 0;

   for(int j = 0 ; j < tablica[iloscLinijekDrugi][iloscLinijekPierwszy] ; j++){
        while(i < pomocnicza->first){
            cout << "- " << p[i] << endl;
            i++;
        }
        while(k < pomocnicza->second){
            cout << "+ " << d[k] << endl;
            k++;
        }

        cout << " " << p[i] << endl;

        pomocnicza++;
        k++;
        i++;
   }
    while(i < iloscLinijekPierwszy){
        cout << "- " << p[i] << endl;
        i++;
    }

    while(k < iloscLinijekDrugi){
            cout << "+ " << d[k] << endl;
            k++;
    }


}


int main () {
  ifstream pierwszy ("pierwszy.txt");
  ifstream drugi ("drugi.txt");

  if (!(pierwszy.is_open() && drugi.is_open())){
    cout << "nie ma pliczku";
    return 1;
  }

    int iloscLinijekPierwszy = policzLinijki(pierwszy),
        iloscLinijekDrugi = policzLinijki(drugi);

    int ** tablica = new int*[iloscLinijekDrugi+1];

    for(int i = 0 ; i <= iloscLinijekDrugi ; i++)
        tablica[i] = new int[iloscLinijekPierwszy+1];

    string * p = new string[iloscLinijekPierwszy];
    uzupelnijTablice(iloscLinijekPierwszy,p,pierwszy);
    string * d = new string[iloscLinijekDrugi];
    uzupelnijTablice(iloscLinijekDrugi,d,drugi);

    pierwszy.close();
    drugi.close();

    cout << "pierwszy plik zawiera: \n\n";
    wypiszTablice(iloscLinijekPierwszy,p);
    cout << "\ndrugi plik zawiera: \n\n";
    wypiszTablice(iloscLinijekDrugi,d);

    przygotujTabliceWynikow(p,iloscLinijekPierwszy,d,iloscLinijekDrugi,tablica);

    delete [] p;
    delete [] d;
    for(int i = 0 ; i < iloscLinijekDrugi ; i++)
        delete [] tablica[i];
    delete [] tablica;

    return 0;
}
