/*
	Lojik Devreler dersinde gördüğümüz fonksiyon sadeleştirme yöntemlerinden olan
	Quine-McCluskey metoduyla fonksiyon sadeleştirmesi yapan program
*/

#include <iostream>  //Kullanıcı ile iletişim için gerekli kütüphane
#include <vector> 	 //Dinamik dizi için gerekli kütüphane 
#include <algorithm> //Sıralama algoritması için gerekli kütüphane
#include <string>    //String sınıfını barındıran kütüphane
#include "windows.h" //Programı bekletmek için gerekli kütüphane

using namespace std;

typedef struct Minterm{
	string binary;
	int group;
	bool check;
};

string toBinary(int,int); //Verilen sayıyı binary'e çevirir
vector<Minterm> compare(vector<Minterm>,vector<Minterm>*); //Verilen listelerle Quine-Mccluskey karşılaştırması yapar

int main()
{
	vector<Minterm> list1,list2,fullList;
	vector<int> rawList;
	
	int mintermSize;
	cout << "Minterm sayisini giriniz:";
	cin  >> mintermSize;

	//Mintermleri kullanıcıdan al
	int rawInputData;
	for(int i=0;i<mintermSize;i++)
	{
		cout << i+1 <<". Mintermi Giriniz:";
		cin  >> rawInputData;
		rawList.push_back(rawInputData);
	}

	//Ham listeyi küçükten büyüğe sırala
	sort(rawList.begin(), rawList.end() );

	//En büyük binary formunun basamak sayısını bul
	int rawListBiggestMember,biggestBinaryLength;
	rawListBiggestMember = rawList[rawList.size()-1];
	string biggestBinary;
	biggestBinary = toBinary(rawListBiggestMember,-1);
	biggestBinaryLength = biggestBinary.length();

	//Alınan ham veriyi veri yapısına aktar
	Minterm temp;
	string  forFindGroup;
	
	for(int j=0; j<mintermSize;j++)
	{
		int counter=0;
		temp.binary  = toBinary(rawList[j],biggestBinaryLength);

		//Stringin içerisindeki digit 1 olunca sayıcıyı 1 arttırır(Binary'nin Grubunu bulur)
		for(int k=0; k<temp.binary.length() ;k++)
		{	
			if(temp.binary[k] == '1')
			{
				counter++;
			}
		}

		temp.group = counter;
		temp.check   = false;
		list1.push_back(temp);
	}

	//NOT:Compare fonksiyonuna verilen liste ile geri dönen liste aynı ise liste sadeleşmiştir.

	//list1 ve list2 aynı mı kontrol ediliyor
	int state = true;

	do
	{
		int sameCounter=0;
		list2 = list1;
		list1 = compare(list1,&fullList);  //Karşılaştırma

		//Listelerin boyutları eşit ise
		if(list1.size() == list2.size() )
		{
			for(int k=0; k<list1.size() ;k++)
			{
				//List1 ve List2'nin k. elemanı eşitse
				if(list1[k].binary == list2[k].binary && list1[k].group == list2[k].group && list1[k].check == list2[k].check)
				{
					sameCounter++;
				}
			}

			//Eğer tüm elemanlar eşit ise
			if( sameCounter==list1.size() )
			{
				state = false;
			}	
		}

	}while(state);

	//Sadeleştirilmiş fonksiyonu verecek olan listeyi yazdır
	cout << "Sadelestirilmis fonksiyon listesi" << endl;
	for(int k=0; k<fullList.size();k++)
	{
		cout << fullList[k].binary << " " << fullList[k].group << " " << fullList[k].check << endl;
	}	

	system("PAUSE"); //Programın aniden kapanmasını engelle

	return 0;
}

string toBinary(int data,int amountDigit)
{
	string r="";
	string temp="";

	//Verilen data'nın 2 ile bölümünden kalanları bulma
	while(data>=1)
	{
		r += (char) (data%2)+48; //ASCII'da 48 sayısı 0'ı 49 sayısı 1'i ifade eder
		data = (int)data/2;		
	}

	//-1 Girilirse basamak eşitleme işlemi istenilmiyordur
	if(amountDigit != -1)
	{
	//Bulunan binary formu en büyük sayının basamak sayısına getirme
		while(amountDigit > r.length() )
		{
			r += "0";
		}
	}

	//r stringinin tersini alma
	for(int i=1; i<=r.length() ;i++)
	{
		temp += r[(r.length()-i)];
	}

	return temp;
}

vector<Minterm> compare(vector<Minterm> list,vector<Minterm>* fullList)
{
	vector<Minterm> returnTemp;
	Minterm temp;
	string temp1,temp2,newTempBinary;
	int checkCounter,groupCounter;

	for (int stable = 0; stable<list.size(); stable++)
	{
		for (int traveler = 0; traveler<list.size(); traveler++)
		{
			//Eğer gruplar arasında 1 fark varsa
			newTempBinary = "";
			if(list[stable].group == (list[traveler].group)-1)
			{
				temp1= list[stable].binary;
				temp2= list[traveler].binary;
				checkCounter = 0;

				//2 Mintermin digitleri karşılaştırılıyor
				for (int i = 0; i < temp1.length(); i++)
				{
					if(temp1[i] == temp2[i])
					{
						newTempBinary += temp1[i];
					}else
					{
						checkCounter++;
						newTempBinary += "X";
					}
				}

				//Eğer sadece 1 digit değişmişse o minterm yeni listeye eklenir ve karşılaştırılan minterm check durumları true yapılır
				if (checkCounter == 1)
				{
					int counter = 0;
					temp.binary = newTempBinary;

					//Stringin içerisindeki digit 1 olunca sayıcıyı 1 arttırır(Binary'nin Grubunu bulur)
					for(int k=0; k<temp.binary.length() ;k++)
					{	
						if(temp.binary[k] == '1')
						{
							counter++;
						}
					}
					temp.group  = counter;
					temp.check  = false;
					returnTemp.push_back(temp);
					list[stable].check   = true;
					list[traveler].check = true;
				}
			}
		}
	}

	//FullList,check durumları false olan mintermleri barındırır
	for (int j = 0; j < list.size(); j++)
	{
		if(list[j].check == false)
		{
			fullList->push_back(list[j]);
		}
	}

	return returnTemp;
}