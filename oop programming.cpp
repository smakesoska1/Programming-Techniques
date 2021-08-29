#include<iostream>
#include<stdexcept>
#include<iomanip>
#include<cstring>
#include<algorithm>
#include<initializer_list>

class Tim
{
	char ime_tima[20];
	int broj_odigranih;
	int broj_pobjeda;
	int broj_nerijesenih;
	int broj_poraza;
	int broj_datih;
	int broj_primljenih;
	int broj_poena;

public:
	Tim(const char ime[]) :broj_odigranih(0),broj_pobjeda(0),broj_nerijesenih(0),broj_poraza(0),broj_datih(0),broj_primljenih(0),broj_poena(0) {
		if(strlen(ime)>20)
			throw std::range_error("Predugacko ime tima");
		strcpy(ime_tima,ime);
	}
	void ObradiUtakmicu(int broj_datih,int broj_primljenih);
	const char *DajImeTima() const {
		const char *pok(ime_tima);
		return pok;
	}
	int DajBrojPoena() const {
		return broj_poena;
	}
	int DajGolRazliku() const {
		return broj_datih-broj_primljenih;
	}
	void IspisiPodatke() const;
};

void Tim::ObradiUtakmicu(int broj_datih,int broj_primljenih)
{
	if(broj_datih<0 || broj_primljenih<0)
		throw std::range_error("Neispravan broj golova");
	else {
		Tim::broj_odigranih++;
		Tim::broj_datih+=broj_datih;
		Tim::broj_primljenih+=broj_primljenih;
		if(broj_datih>broj_primljenih) {
			Tim::broj_pobjeda++;
			Tim::broj_poena+=3;
		} else if(broj_datih<broj_primljenih)
			Tim::broj_poraza++;
		else {
			Tim::broj_nerijesenih++;
			Tim::broj_poena+=1;
		}

	}
}
void Tim::IspisiPodatke() const
{
	std::cout<<std::fixed<<std::left<<std::setw(20)<<DajImeTima();
	std::cout<<std::right<<std::setw(4)<<broj_odigranih<<std::setw(4)<<broj_pobjeda<<std::setw(4)<<broj_nerijesenih<<std::setw(4)<<broj_poraza<<std::setw(4)<<broj_datih<<std::setw(4)<<broj_primljenih<<std::setw(4)<<broj_poena<<std::endl;
}

class Liga
{
	int broj_timova;
	const int max_br_timova;
	Tim **timovi;
	static bool VeciGoRazlike(Tim *tim1,Tim *tim2) {
		if(tim1->DajBrojPoena()==tim2->DajBrojPoena()) {
			if(tim1->DajGolRazliku()==tim2->DajGolRazliku())
				return (strcmp(tim1->DajImeTima(),tim2->DajImeTima())<0);
			return tim1->DajGolRazliku()>tim2->DajGolRazliku();
		}
		return tim1->DajBrojPoena()>tim2->DajBrojPoena();
	}
public:
	explicit Liga(int velicina_lige) :broj_timova(0),max_br_timova(velicina_lige),timovi(new Tim*[max_br_timova] {}) {}
	explicit Liga(std::initializer_list<Tim> lista_timova):broj_timova(lista_timova.size()),max_br_timova(lista_timova.size()) {
		timovi=new Tim*[lista_timova.size()] {};
		std::initializer_list<Tim>::iterator i;
		int j(0);
		for(i=lista_timova.begin(); i!=lista_timova.end(); i++) {
			timovi[j]=new Tim(*i);
			j++;
		}

	}
	~Liga() {
		for(int i=0; i<max_br_timova; i++)
			delete timovi[i];
		delete [] timovi;
	}
	Liga(const Liga &l) :broj_timova(l.broj_timova),max_br_timova(l.max_br_timova),timovi(new Tim*[max_br_timova] {}) {
		for(int i=0; i<broj_timova; i++) {
			this->timovi[i]=new Tim(*l.timovi[i]);

		}
	}
	Liga(Liga &&l) :broj_timova(l.broj_timova),max_br_timova(l.max_br_timova),timovi(l.timovi) {
		l.timovi=nullptr;
	}
	Liga &operator=(const Liga &l) {
		if(l.max_br_timova!=max_br_timova)
			throw std::logic_error("Nesaglasni kapaciteti liga");
		if(broj_timova<l.broj_timova) {
			try {
				for(int i=broj_timova; i<l.broj_timova; i++) {
					timovi[i]=new Tim(*l.timovi[i]);
				}
			} catch(...) {
				for(int i=broj_timova; i<l.broj_timova; i++) {
					delete timovi[i];
					timovi[i]=nullptr;
				}
				throw;
			}
		} else {
			for(int i=l.broj_timova; i<broj_timova; i++) {
				delete timovi[i];
				timovi[i]=nullptr;
			}
		}
		broj_timova=l.broj_timova;
		for(int i=0; i<broj_timova; i++) {
			*timovi[i]=*l.timovi[i];
		}
		return *this;
	}
	Liga &operator =(Liga &&l) {
		if(l.max_br_timova!=max_br_timova)
			throw std::logic_error("Nesaglasni kapaciteti liga");
		std::swap(broj_timova,l.broj_timova);
		std::swap(timovi,l.timovi);
		return *this;
	}
	void DodajNoviTim(const char ime_tima[]);
	void RegistrirajUtakmicu(const char tim1[],const char tim2[],int rezultat_1,int rezultat_2);
	void IspisiTabelu();
};
void Liga::DodajNoviTim(const char ime_tima[])
{
	if(broj_timova==max_br_timova)
		throw std::range_error("Liga popunjena");
	timovi[broj_timova++]=new Tim(ime_tima);

}
void Liga::RegistrirajUtakmicu(const char tim1[],const char tim2[],int rezultat_1,int rezultat_2)
{
	int koliko=0;
	for(unsigned i=0; i<broj_timova; i++)
		if(std::strcmp(timovi[i]->DajImeTima(),tim1)==0)
			koliko++;
	if(!koliko)
		throw std::logic_error("Tim nije nadjen");
	koliko=0;
	for(unsigned i=0; i<broj_timova; i++)
		if(std::strcmp(timovi[i]->DajImeTima(),tim2)==0)
			koliko++;
	if(!koliko)
		throw std::logic_error("Tim nije nadjen");
	for(int i=0; i<broj_timova; i++) {
		if(!strcmp(timovi[i]->DajImeTima(),tim1))
			timovi[i]->ObradiUtakmicu(rezultat_1,rezultat_2);
		if(!strcmp(timovi[i]->DajImeTima(),tim2))
			timovi[i]->ObradiUtakmicu(rezultat_2,rezultat_1);


	}

}
void Liga::IspisiTabelu()
{
	std::sort(timovi,timovi+broj_timova,VeciGoRazlike);
	for(int i=0; i<broj_timova; i++)
		timovi[i]->IspisiPodatke();
}
int main()
{
	return 0;
}
