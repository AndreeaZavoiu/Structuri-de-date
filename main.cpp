#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>  // pt swap si sort
#include <chrono>

using namespace std::chrono;
using namespace std;
ifstream fin("date.txt");

//BubbleSort
void BubbleSort (vector<int> &v, int n)
{

    int i, j;
    bool flag=false;
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n-i-1; j++)
            if( v[j] > v[j+1])
            {
                swap(v[j], v[j+1]);
                flag = true;
            }
        if(flag==false)
            break;
    }
}

//CountSort
void CountSort(vector<int> &v, int n)
{
    int maxi = v[0];
    for (auto &i:v)
        if (i > maxi)
            maxi = i;

    vector<int> count(maxi + 1);

    for (int i = 0; i < n; i++)
        count[v[i]]++;

    int poz(0);
    for (int i = 0; i <= maxi; i++) {
        for (int j = 0; j < count[i]; j++)
            v[poz++] = i;
    }
}

//RadixSort
void RadixSort10(vector<int>& v, int n)
{
    vector<int> vnou[10]; // bucketuri pentru cele 10 cifre
    int p10=1;            // puterile lui 10 -> plec de la 0
    bool sortat=false;    // ca sa stiu cat timp continui sortarea
    while (!sortat)
    {
        sortat=true;                  // presupun ca e sortat
        for (int i=0; i<n; i++)       // pt ca lucram in baza 10
        {
            int uc=(v[i]/p10) %10;    // ultima cifra
            vnou[uc].push_back(v[i]); // in bucketul pentru cifra actuala punem elementele din vector care o contin
        }
        int poz=0;
        for (auto &i:vnou) {        // parcurg bucketurile din vnou
            for (auto &j:i)         // parcurg elementele din fiecare bucket
            {
                v[poz]=j;           // refac vectorul v cu elementele din sortarea curenta
                if (poz>0 && v[poz] < v[poz-1])     // "tin evidenta" daca a ajuns sortat corect sau nu
                    sortat=false;
                poz++;
            }
            i.clear();          // trebuie sa eliberez bucketul curent pentru ca il voi umple altfel la urmatorul loop -> se schimba cifra dupa care fac sortarea
        }
        p10*=10;
    }
}
void RadixSort256(vector<int>& v, int n)
{
    vector<int> vnou[256];
    int shift = 0;
    const int mask = 255;    // = 11111111 = (8 de 1) in baza 2
    bool sortat = false;    // ca sa stiu cat timp continui sortarea
    while (!sortat)
    {
        sortat=true;                  // presupun ca e sortat
        for (int i=0; i<n; i++)       // pt ca lucram in baza 10
        {
            int uc = (v[i] >> 256) & mask;    // ultima cifra
            vnou[uc].push_back(v[i]); // in bucketul pentru cifra actuala punem elementele din vector care o contin
        }
        int poz=0;
        for (auto &i:vnou) {        // parcurg bucketurile din vnou
            for (auto &j:i)         // parcurg elementele din fiecare bucket
            {
                v[poz]=j;           // refac vectorul v cu elementele din sortarea curenta
                if (poz>0 && v[poz] < v[poz-1])     // "tin evidenta" daca a ajuns sortat corect sau nu
                    sortat=false;
                poz++;
            }
            i.clear();          // trebuie sa eliberez bucketul curent pentru ca il voi umple altfel la urmatorul loop -> se schimba cifra dupa care fac sortarea
        }
        shift+=8;
    }
}

void Merge(vector<int> &v, int st, int mij, int dr)
{
    int i = st, j = mij+1, l = 1;
    vector<int> aux;
    while(i <= mij && j <= dr)
        if(v[i] < v[j]) aux.push_back(v[i++]);
        else aux.push_back(v[j++]);
    while(i <= mij) aux.push_back(v[i++]);
    while(j <= dr) aux.push_back(v[j++]);
    for(int i = st; i <= dr; ++i) v[i] = aux[i-st];
}
void MergeSort(vector<int> &v, int st, int dr)
{
    if(dr > st)
    {
        int mij = (st+dr)/2;
        MergeSort(v, st, mij);
        MergeSort(v, mij+1, dr);
        Merge(v, st, mij, dr);
    }
}
//QuickSort
void QuickSortMij(vector<int> &v, int inf, int sup)
{
    int i=inf, j=sup, mij, t;
    mij = v[(i + j) / 2];
    while (i <= j) {
        while (v[i] < mij)
            i++;
        while (v[j] > mij)
            j--;
        if (i <= j) {
            t = v[i];
            v[i] = v[j];
            v[j] = t;
            i++; j--;
        }
    }
    if (j > inf)
        QuickSortMij(v, inf, j);
    if (i < sup)
        QuickSortMij(v, i, sup);
}

//Verificare sortare corecta
vector<int> frecventa (vector<int> &v, int n)
{
    int maxi = v[0];
    for (auto &i:v)
        if (i > maxi)
            maxi = i;
    vector<int> count(maxi + 1);
    for (int i = 0; i < v.size(); i++)
        count[v[i]]++;
    return count;
}
bool test_sort(vector<int> v, vector<int> w, int n) // dureaza mult pe teste mari pt ca e in O(n^2)
{
    // verific sa am acelasi numar de elemente in vector
    if (n!=w.size())
        return false;

    // verific ordinea crescatoare a elementelor .. O(n^2)
    for (int i=0; i<n-1; i++)
        for (int j=i; j<n; j++)
            if (w[i]>w[j])
                return false;

    vector<int> fr;
     fr = frecventa(w, n);
	// verific ca valorile elementelor sa nu fi fost modificate
    for (int i=0; i<n; i++)
    {
        fr[w[i]]--;
        if (fr[w[i]]<0)
            return false;
    }

    for (int i=0; i<n; i++)
        if (fr[w[i]]>0)
            return false;

    return true;
}

int main()
{
    int T, t, n, Max, x, i, j;
    vector <int> v;

    fin>>T;
    //Pentru fiecare test din cele T teste
    for (t=1; t<=T; t++)
    {
        fin>>n>>Max;
        cout<<endl<<"Testul "<<t<<": N = "<<n<<"  ValMax = "<<Max<<endl;

        // Generez vectorul
        for (i=0; i<n; i++)
            v.push_back(rand() % Max + 1);

        //Pentru fiecare din cei 8 algoritmi:
        for (j=0; j<8; j++)
        {
            //Sortez o copie a vectorului initial pentru a nu-l pierde de la primul test si pentru a putea verifica la sfarsit daca a fost sortat corect
            vector<int> copieV(v);      // tot cu constructorul de copiere
            if (j==0)
            {
                if (Max < 1000000 && n < 1000000) //deci putem face CountSort
                {
                    auto start = high_resolution_clock::now();
                    CountSort(copieV, copieV.size());
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<microseconds>(stop - start);
                    if (test_sort(v, copieV, v.size()) == 0)
                        cout<<"Vectorul nu a fost sortat corect!"<<endl;
                    else cout<<"CountSort: "<< duration.count()<<" microsecunde" << endl;
                }
                else { cout<<"CountSort nu merge pe testul "<<t<<endl; continue;}
            }

            else if (j==1)
            {
                if (n < 10000)
                {
                    auto start = high_resolution_clock::now();
                    BubbleSort(copieV, copieV.size());
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<microseconds>(stop - start);
                    if (test_sort(v, copieV, v.size()) == 0)
                        cout<<"Vectorul nu a fost sortat corect!\n";
                    else cout<<"BubbleSort: "<< duration.count()<<" microsecunde\n";
                }
                else {cout<<"BubbleSort nu merge pe testul "<<t<<endl; continue;}
            }
            else if (j==2)
            {
                if (n < 1000000)
                {
                auto start = high_resolution_clock::now();
                RadixSort10(copieV, copieV.size());
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(stop - start);
                if (test_sort(v, copieV, v.size()) == 0)
                    cout<<"Vectorul nu a fost sortat corect!\n";
                else cout<<"RadixSort in baza 10: "<< duration.count()<<" microsecunde\n";
                }
                else cout<<"RadixSort in baza 10 nu merge pe testul "<<t<<endl;
            }
            else if (j==3)
            {/*
                auto start = high_resolution_clock::now();
                RadixSort256(copieV, copieV.size());
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(stop - start);
                if (test_sort(v, copieV, v.size()) == 0)
                    cout<<"Vectorul nu a fost sortat corect!"<<endl;
                else cout<<"RadixSort in baza 256: -\n";
            */}
            else if (j==4)
            {
                if (n < 1000000)
                {
                auto start = high_resolution_clock::now();
                MergeSort(copieV, 0, copieV.size());
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(stop - start);
                if (test_sort(v, copieV, v.size()) == 0)
                    cout<<"Vectorul nu a fost sortat corect!"<<endl;
                else cout<<"MergeSort: "<< duration.count()<<" microsecunde\n";
                }
            }
            else if (j==5)
            {
                auto start = high_resolution_clock::now();
                QuickSortMij(copieV, 0, copieV.size()-1);
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(stop - start);
                cout<<"QuickSort cu pivot la mijloc: ";
                if (test_sort(v, copieV, v.size()) == 0)
                    cout<<"Vectorul nu a fost sortat corect!"<<endl;
                else cout<<duration.count()<<" microsecunde\n";
            }
            else if (j==6)
            {
                /*auto start = high_resolution_clock::now();
                QuickSortMed3(copieV, 0, copieV.size()-1);
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(stop - start);
                if (test_sort(v, copieV, v.size()) == 0)
                    cout<<"Vectorul nu a fost sortat corect!"<<endl;
                else cout<<"QuickSort cu pivot mediana din 3: "<< duration.count()<<" microsecunde\n";
            */}
            else
            {
                auto start = high_resolution_clock::now();
                std::sort(copieV.begin(), copieV.end());        // in O(nlogn)
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(stop - start);
                if (test_sort(v, copieV, v.size()) == 0)
                    cout<<"Vectorul nu a fost sortat corect!\n";
                else cout<<"Sortarea din STL: "<< duration.count()<<" microsecunde\n";
            }

        }
        v.clear();
    }
    return 0;
}
