#include <iostream>
#include<stdexcept>
#include<string>
#include<vector>
#include<algorithm>


class ApstraktniStudent
{
	private:
	std::string ime;
	std::string prezime;
	int indeks;
	int broj_polozenih;
	double prosjek;
	public:
	ApstraktniStudent(std::string ime,std::string prezime,int indeks) :ime(ime),prezime(prezime),indeks(indeks),broj_polozenih(0),prosjek(5){}
	virtual ~ApstraktniStudent(){}
	std::string DajIme() const {return ime;}
	std::string DajPrezime() const {return prezime;}
	int DajBrojIndeksa() const{return indeks;}
	int DajBrojPolozenih() const{return broj_polozenih;}
	double DajProsjek() const {return prosjek;}
	void RegistrirajIspit(int ocjena);
	void PonistiOcjene(){prosjek=5;broj_polozenih=0;}
	virtual void IspisiPodatke() const{}
	virtual ApstraktniStudent *DajKopiju() const{return nullptr;}
};
void ApstraktniStudent::RegistrirajIspit(int ocjena)
{
	if(ocjena<5 || ocjena>10)
	throw std::domain_error("Neispravna ocjena");
	if(ocjena>5)
	{
		broj_polozenih++;
		prosjek=(DajProsjek()*(broj_polozenih-1)+ocjena)/broj_polozenih;
	}
}
class StudentBachelor:public ApstraktniStudent
{
	public:
	StudentBachelor(std::string ime,std::string prezime,int indeks):ApstraktniStudent(ime,prezime,indeks){}
	StudentBachelor *DajKopiju() const override{
		return new StudentBachelor(*this);
	}
	void IspisiPodatke() const override;
};
void StudentBachelor::IspisiPodatke() const{
	std::cout<<"Student bachelor studija "<<DajIme()<<" "<<DajPrezime()<<", sa brojem indeksa "<<DajBrojIndeksa()<<","<<std::endl<<"ima prosjek "<<DajProsjek()<<"."<<std::endl;
}
class StudentMaster:public ApstraktniStudent
{
	int bachelor_studij;
	public:
	StudentMaster(std::string ime,std::string prezime,int indeks,int bachelor_studij):ApstraktniStudent(ime,prezime,indeks),bachelor_studij(bachelor_studij){}
	StudentMaster *DajKopiju() const override{
		return new StudentMaster(*this);
	}
	void IspisiPodatke() const override;
};
void StudentMaster::IspisiPodatke() const{
	std::cout<<"Student master studija "<<DajIme()<<" "<<DajPrezime()<<", sa brojem indeksa "<<DajBrojIndeksa()<<","<<std::endl<<"zavrsio bachelor studij godine "<<bachelor_studij<<",ima prosjek "<<DajProsjek()<<"."<<std::endl;
}
class Fakultet
{
	std::vector<ApstraktniStudent*>studenti;
	public:

	Fakultet(){};
	~Fakultet();
	Fakultet(const Fakultet &f);
	Fakultet(Fakultet &&f);
	Fakultet &operator=(const Fakultet &f);
	Fakultet &operator=(Fakultet &&f);
	void UpisiStudenta(std::string ime,std::string prezime,int indeks);
	void UpisiStudenta(std::string ime,std::string prezime,int indeks,int godina);
	void ObrisiStudenta(int indeks);
	ApstraktniStudent &operator [](int indeks);
	const ApstraktniStudent &operator [](int indeks) const;
	void IspisiSveStudente() const;
};
Fakultet::~Fakultet(){


	for(int i=0;i<studenti.size();i++)
	delete studenti[i];
}
Fakultet::Fakultet(const Fakultet &f)
{
	for(int i=0;i<f.studenti.size();i++)
	studenti.push_back(f.studenti[i]->DajKopiju());
}
Fakultet::Fakultet(Fakultet &&f)
{
	studenti=f.studenti;
	f.studenti.resize(0);
}
Fakultet& Fakultet::operator=(const Fakultet &f)
{
	int brstudenata(0);
	for(int i=0;i<f.studenti.size();i++)
	{
		if(f.studenti[i])
		brstudenata++;
	}
	for(int i=0;i<studenti.size();i++){
	delete studenti[i];
	 studenti[i]=nullptr;}
	 int indeks(0);
	 studenti.resize(brstudenata);
	 for(int i=0;i<f.studenti.size();i++)
	 {
	 	if(f.studenti[i]){
	 	studenti[indeks]=f.studenti[i]->DajKopiju();
	 	indeks++;}
	 }
	 return *this;
}
Fakultet &Fakultet::operator=(Fakultet &&f)
{
	for(int i=0;i<studenti.size();i++)
	{
		delete studenti[i];
		studenti[i]=nullptr;
	}
	studenti.resize(0);
	std::swap(studenti,f.studenti);
	return *this;
}
 ApstraktniStudent& Fakultet::operator[](int indeks)
{
	auto it=std::find_if(studenti.begin(),studenti.end(),[indeks](ApstraktniStudent* student){
		if(student->DajBrojIndeksa()==indeks)
		return true;
		return false;
	});
	if(it==studenti.end()) throw std::logic_error("Student sa zadanim brojem indeksa ne postoji");
	return **it;
}
 const ApstraktniStudent& Fakultet::operator[](int indeks) const
{
	auto it=std::find_if(studenti.begin(),studenti.end(),[indeks](ApstraktniStudent* student){
		if(student->DajBrojIndeksa()==indeks)
		return true;
		return false;
	});
	if(it==studenti.end()) throw std::logic_error("Student sa zadanim brojem indeksa ne postoji");
	return **it;
}
void Fakultet::IspisiSveStudente() const
{
	Fakultet pom=*this;
	std::sort(pom.studenti.begin(),pom.studenti.end(),[](ApstraktniStudent *s1,ApstraktniStudent *s2){
		if(s1->DajProsjek()>s2->DajProsjek())
		return true;
		else if(s1->DajProsjek()<s2->DajProsjek())
		return false;
		else
		{
			return s1->DajBrojIndeksa()<s2->DajBrojIndeksa();
		}});
		for(auto x:pom.studenti)
		x->IspisiPodatke();
}
void Fakultet::UpisiStudenta(std::string ime,std::string prezime,int indeks)
{
	auto it=std::find_if(studenti.begin(),studenti.end(),[indeks](ApstraktniStudent *s1){
		if(indeks==s1->DajBrojIndeksa())
		return true;
		return false;
	});
	if(it!=studenti.end())
	throw std::logic_error("Student sa zadanim brojem indeksa vec postoji");
	studenti.push_back(new StudentBachelor(ime,prezime,indeks));
}
void Fakultet::UpisiStudenta(std::string ime,std::string prezime,int indeks,int godina)
{
	auto it=std::find_if(studenti.begin(),studenti.end(),[indeks](ApstraktniStudent *s1){
		if(indeks==s1->DajBrojIndeksa())
		return true;
		return false;
	});
	if(it!=studenti.end())
	throw std::logic_error("Student sa zadanim brojem indeksa vec postoji");
	studenti.push_back(new StudentMaster(ime,prezime,indeks,godina));
}
void Fakultet::ObrisiStudenta(int indeks)
{
	auto it=std::find_if(studenti.begin(),studenti.end(),[indeks](ApstraktniStudent *s1){
		if(indeks==s1->DajBrojIndeksa())
		return true;
		return false;
	});
	if(it==studenti.end())
	throw std::logic_error("Student sa zadanim brojem indeksa ne postoji");
	for(int i=0;i<studenti.size();i++)
	{
		if(indeks==studenti[i]->DajBrojIndeksa())
		delete studenti[i],studenti.erase(studenti.begin()+i);
	}
}

int main ()
{
	Fakultet etf;
	etf.UpisiStudenta("Sara","Dzebo",17814);
	etf[17814].RegistrirajIspit(8);
	etf.UpisiStudenta("Benjamin","Mezit",17855);
	etf[17855].RegistrirajIspit(10);
	etf.IspisiSveStudente();
	return 0;
}
