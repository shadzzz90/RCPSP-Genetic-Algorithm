#include<iostream>
#include<vector>
#include<regex>
#include<fstream>
#include<sstream>
#include<string>
#include<cstdlib>
#include<ctime>
#include<algorithm>
#include<random>
#include<utility>
#include<map>
#include<functional>
#include<array>
#include "dirent.h"
#include<time.h>


#define POPSIZE 40

#define Pmutation 0.03

#define Pcrossover 0.8

#define JOBSIZE 32

#define GENERATIONS 25


using namespace std;

random_device rd;

mt19937 gen(rd());

uniform_real_distribution<float> dist(0,1);

hello

class Job
{	
	public:
		
		int num;
		int duedate;
		int duration;
		int start_time;
		int finish_time;
		vector <int> sucessor;
		vector <int>predecessor;
		float heuristic;
		vector <int> req_renew;
		vector <int> req_nonrenew;
		int no_sucessor;
	
public:
	Job()
	{    
		no_sucessor=0;
		duedate=0;
		 num=0;
		duration=0;
		start_time=0;
		finish_time=0;
		heuristic=0;
		
	}


};

class Individual
{	public:
	
array<Job,JOBSIZE>job;
	
	int makespan;
	int nrenew;
	int nnonrenew;
	vector <int> trenew ;
	vector <int>tnonrenew ;


	

/*
	Individual()
	{
		job = new Job [150];

		makespan=0;
	}

	~Individual()
	{

	}
	*/
	
};

class Population
{ 	
public:
	
	array<Individual,POPSIZE> individual;



	/* 
  public:

	Population()
	{  
		individual= new Individual [100];}

	~Population()
	{

	}
	*/

};

/*bool found_element(vector<int> a,int b)
{
	for(vector<int>::iterator it=a.begin();it!=a.end();it++)
	{
		if(*it==b)
			return true;
		else
			return false;
	}
}
*/


bool nonrenew_available(vector<int> total_nonrenew_resources,vector<int> reqiured_nonrenew)
{
	vector<int>::iterator it1;
	vector<int>::iterator it2;
	vector<int>a;

	for(it1=total_nonrenew_resources.begin(),it2=reqiured_nonrenew.begin();it1!=total_nonrenew_resources.end(),it2!=reqiured_nonrenew.end();it1++,it2++)
	{
		if(*it2<=*it1)
			a.push_back(1);
		else
			a.push_back(0);
	}

	if (all_of(a.begin(),a.end(),[](int x){return(x==1);})|| reqiured_nonrenew.empty())
		return true;

	else
		return false;

				}

void resource_map_inserter(map<int,vector<int>> &resource_map,vector<int>resource)
{
	for(int i=0;i<500;i++)
		resource_map.insert(make_pair(i,resource));
}

void resource_update(int start, int duration,map<int,vector<int>> &resource_map,vector<int>resource)
{

	for(int i=start;i<=duration;i++)
		transform(resource_map.at(i).begin(),resource_map.at(i).end(),resource.begin(),resource_map.at(i).begin(),minus<int>());

}

bool resource_available(int a,int c,map<int,vector<int>> &resource_map, vector<int> resource)
{
	vector<int>::iterator it1;
	vector<int>::iterator it2;
	vector<int> b;
	for(int i=a;i<=c;i++)
	{
		 for(it1=resource_map.at(i).begin(),it2=resource.begin();it1!=resource_map.at(i).end(),it2!=resource.end();it1++,it2++)
	    {
		if(*it2<=*it1)
			b.push_back(1);
		else
			b.push_back(0);
    	}

	}

	if (all_of(b.begin(),b.end(),[](int x){return(x==1);}))
			
			return true;
	else
		return false;

}

		void scheduling(Population &pop, int njobs,Individual seed_individual)
	{
		vector<int> index;
		vector<int>pred;
		vector<int>pred_finish_time;
		map<int,vector<int>> resource_map;
		vector<int> total_nonrenew_resource;
		total_nonrenew_resource=seed_individual.tnonrenew;
		
		for(int i=0;i<POPSIZE;i++)
		{
			
			resource_map_inserter(resource_map,seed_individual.trenew);
			pop.individual[i].job[0].finish_time=0;


			for(int j=1;j<(njobs);j++)
			{
				
				pred=pop.individual[i].job[j].predecessor;

				//copy(pop.individual[i].job[j].predecessor.begin(),pop.individual[i].job[j].predecessor.end(),back_inserter(pred));
//copying the index of predecessor

				for(int k=0; k<(njobs);k++)
				{
					
				for (int n:pred)
					{
						if(pop.individual[i].job[k].num==n)
							
							index.push_back(k);
					}
				}
		
	//copying the finish time 

			for(int n:index)
				pred_finish_time.push_back(pop.individual[i].job[n].finish_time);

			if(equal(pred_finish_time.begin()+1,pred_finish_time.end(),pred_finish_time.begin()) || all_of(pred_finish_time.begin(),pred_finish_time.end(),[](int x){ return (x==0);}) || (pred_finish_time.size()==1))
				{
				pop.individual[i].job[j].start_time=pred_finish_time.front();
				pop.individual[i].job[j].finish_time=(pop.individual[i].job[j].start_time+pop.individual[i].job[j].duration);
				}
				else
				{
				pop.individual[i].job[j].start_time=*max_element(pred_finish_time.begin(),pred_finish_time.end());
				pop.individual[i].job[j].finish_time=(pop.individual[i].job[j].start_time+pop.individual[i].job[j].duration);
				
				}

				

			if(resource_available((pop.individual[i].job[j].start_time),(pop.individual[i].job[j].finish_time),resource_map,pop.individual[i].job[j].req_renew) && nonrenew_available(total_nonrenew_resource,pop.individual[i].job[j].req_nonrenew))
			{
				pop.individual[i].job[j].finish_time=(pop.individual[i].job[j].start_time+pop.individual[i].job[j].duration);
				transform(total_nonrenew_resource.begin(),total_nonrenew_resource.end(),pop.individual[i].job[j].req_nonrenew.begin(),total_nonrenew_resource.begin(),minus<int>());
				resource_update((pop.individual[i].job[j].start_time),(pop.individual[i].job[j].finish_time-1),resource_map,pop.individual[i].job[j].req_renew);
			}
			
			else

				for(int l=0;;l++)
				{
					pop.individual[i].job[j].start_time=(pop.individual[i].job[j].start_time+1);
					pop.individual[i].job[j].finish_time=(pop.individual[i].job[j].start_time+pop.individual[i].job[j].duration);

					if(resource_available(pop.individual[i].job[j].start_time,pop.individual[i].job[j].finish_time,resource_map,pop.individual[i].job[j].req_renew) && nonrenew_available(total_nonrenew_resource,pop.individual[i].job[j].req_nonrenew))
					{
						pop.individual[i].job[j].finish_time=(pop.individual[i].job[j].start_time+pop.individual[i].job[j].duration);
						transform(total_nonrenew_resource.begin(),total_nonrenew_resource.end(),pop.individual[i].job[j].req_nonrenew.begin(),total_nonrenew_resource.begin(),minus<int>());
						resource_update(pop.individual[i].job[j].start_time,(pop.individual[i].job[j].finish_time-1),resource_map,pop.individual[i].job[j].req_renew);

						break;
					}

					else
						continue;
				}

			index.clear();
			pred_finish_time.clear();
			pred.clear();

			}	
		
		resource_map.clear();
		pop.individual[i].makespan=pop.individual[i].job[njobs-1].finish_time;

		}

		}

void Mutation(Population &pop)
{ 

	random_device rd;
	mt19937 eng(rd());
	uniform_int_distribution<int> dist(2,(JOBSIZE-3));
	uniform_int_distribution<int> dist2(0,(POPSIZE-1));
	int random1,random2,random_mut_ind;
	vector<int> random_vec;
	vector<int> vector1;
	vector<int> vector2;
	Job temp;
	
	random_mut_ind=dist2(eng);

	
	for(int k=0; ;k++)
	{ 

		random_vec.clear();
		vector1.clear();
		vector2.clear();
		random_vec.push_back(dist(eng));
	random_vec.push_back(dist(eng));
	std::sort(random_vec.begin(),random_vec.end());
	random1=random_vec.front();
	random2=random_vec.at(1);

		for(int i=0;i<random1;i++)
		vector1.push_back(pop.individual[random_mut_ind].job[i].num);

	for(int j=0;j<random2;j++)
		vector2.push_back(pop.individual[random_mut_ind].job[j].num);

	std::sort(vector1.begin(),vector1.end());
	std::sort(vector2.begin(),vector2.end());
	std::sort(pop.individual[random_mut_ind].job[random1].predecessor.begin(),pop.individual[random_mut_ind].job[random1].predecessor.end());
	std::sort(pop.individual[random_mut_ind].job[random2].predecessor.begin(),pop.individual[random_mut_ind].job[random2].predecessor.end());

	if(includes(vector1.begin(),vector1.end(),pop.individual[random_mut_ind].job[random2].predecessor.begin(),pop.individual[random_mut_ind].job[random2].predecessor.end()) && includes(vector2.begin(),vector2.end(),pop.individual[random_mut_ind].job[random1].predecessor.begin(),pop.individual[random_mut_ind].job[random1].predecessor.end()))
	{

		temp=pop.individual[random_mut_ind].job[random1];
		pop.individual[random_mut_ind].job[random1]=pop.individual[random_mut_ind].job[random2];
		pop.individual[random_mut_ind].job[random2]=temp;
		break;
	}
	else
		continue;

	}

}



void vector_to_array(Individual &child1,Individual &child2, Individual seed_individual,vector<int> son,vector<int> daughter)
{
	for(int i=0;i<JOBSIZE;i++)
	{
		for(int j=0;j<JOBSIZE;j++)
		{
			if(son[i]==seed_individual.job[j].num)
				
				child1.job[i]=seed_individual.job[j];
			
			else
				continue;
		}

	}

	for(int i=0;i<JOBSIZE;i++)
	{
		for(int j=0;j<JOBSIZE;j++)
		{
			if(daughter[i]==seed_individual.job[j].num)
				
				child2.job[i]=seed_individual.job[j];
			
			else
				continue;
		}

	}


}


void sorter(Population &pop)
{
	Individual tempind;

	for(int i=0;i<POPSIZE;i++)
	{
		for(int j=0;j<(POPSIZE-1);j++)
		{
			if(pop.individual[i].makespan<pop.individual[j].makespan)
			{ 
				tempind=pop.individual[i];
				pop.individual[i]=pop.individual[j];
				pop.individual[j]=tempind;
			}
			
		}
	}

}

void Crossover (vector<int>father,vector<int> mother,vector<int> &son,vector<int>&daughter)
{
	random_device rd;
	mt19937 eng(rd());
	uniform_int_distribution<int> dist(2,(JOBSIZE-3));
	int random1,random2;
		vector<int>child;
		vector<int>random_vec;
		random_vec.push_back(dist(eng));
		random_vec.push_back(dist(eng));
		std::sort(random_vec.begin(),random_vec.end());
		random1=random_vec.front();
		random2=random_vec.at(1);
		
		for(int k=0;k<random1;k++)
		{
			son.push_back(father[k]);
			daughter.push_back(mother[k]);
		}

		int temp=0;

		// from mother to son
		for(int i=random1;i<random2;i++)
		{   
			int temp=0;

		for(vector<int>::iterator it=mother.begin();it!=mother.end();it++)
		{
			vector<int>::iterator it1=std::find(son.begin(),son.end(),*it);

				if(it1!=son.end())
					continue;

				else
				{ temp=(*it);
					break;}
		}
		son.push_back(temp);
		
		}

		//from father to son
		for(int j=random2;j<JOBSIZE;j++)
		{
			int temp=0;
			for(vector<int>::iterator it=father.begin();it!=father.end();it++)
			{
				vector<int>::iterator it1=std::find(son.begin(),son.end(),*it);

				if(it1!=son.end())
					continue;
				else
				{  temp=(*it);
					break;
				}

			}

			son.push_back(temp);
		}

	//from father to daughter
		for(int i=random1;i<random2;i++)
		{ 
			int temp=0;

			for(vector<int>::iterator it=father.begin();it!=father.end();it++)
			{
				vector<int>::iterator it1=std::find(daughter.begin(),daughter.end(),*it);

				if(it1!=daughter.end())
					continue;

				else
				{  temp=(*it);
				break;}
			}

			daughter.push_back(temp);
		}

		// from mother to daughter

		for(int j=random2;j<JOBSIZE;j++)
		{
			int temp=0;

			for(vector<int>::iterator it=mother.begin();it!=mother.end();it++)
			{
				vector<int>::iterator it1=std::find(daughter.begin(),daughter.end(),*it);

				if(it1!=daughter.end())
					continue;

				else
				{temp=(*it);
				break;}
			}

			daughter.push_back(temp);
		}


		
}


void array_to_vector(Individual ind,vector<int> &a)
{
	for(int i=0;i<JOBSIZE;i++)
		a.push_back(ind.job[i].num);
}

void vector_to_array2(vector<int>&a,Individual seed_individual,Individual &ind)
{
	for(int i=0;i<JOBSIZE;i++)
	{
		for(int j=0;j<JOBSIZE;j++)
		{
			if(a[i]==seed_individual.job[j].num)
			{ 
				ind.job[i]=seed_individual.job[j];
				break;
			}
			else 
				continue;
		}

	}
	
	}


int Solver(Population &pop1, Individual seed_individual)
{
	random_device rd;
	mt19937 eng(rd());

	//uniform_int_distribution<int> dist1(0,(POPSIZE-1));

	uniform_int_distribution<int> dist1(0,((POPSIZE/2)-1));
	uniform_real_distribution<float>dist2(0,1);
	uniform_int_distribution<int>dist3(0,POPSIZE);
	vector<Population> pop(GENERATIONS);
	pop[0]=pop1;
	Individual child1;
	Individual child2;
	vector<int> father;
	vector<int> mother;
	vector<int>son;
	vector<int>daughter;
	int random_ind1,random_ind2;
	int optimal_soln=0;
	 

	for(int i=0;i<(GENERATIONS-1);i++)
	{ 
		sorter(pop[i]);

	 for(int l=0;l<(POPSIZE/2);l++)
		{pop[i+1].individual[l]=pop[i].individual[l];} 



		int x=0;
		for(int j=(POPSIZE/2),k=((3*POPSIZE)/4);x<(POPSIZE/4) ;j++,k++)
		{
			random_ind1=dist1(eng);
			random_ind2=dist1(eng);
			x++;

			float a=dist2(eng);
			 
			if(a<Pcrossover)
			{
				array_to_vector(pop[i+1].individual[random_ind1],father);
				array_to_vector(pop[i+1].individual[random_ind2],mother);
				Crossover(father,mother,son,daughter);
				vector_to_array(child1,child2,seed_individual,son,daughter);
				pop[i+1].individual[j]=child1;
				pop[i+1].individual[k]=child2;

			}
			else
			{
				pop[i+1].individual[j]=pop[i+1].individual[random_ind1];
				pop[i+1].individual[k]=pop[i+1].individual[random_ind2];
			}
			
			father.clear();
			mother.clear();
			son.clear();
			daughter.clear();
		} 

	
		
	/*	int x=0;
		for(int j=0,l=(POPSIZE/2);x<(POPSIZE/2);j++,l++)
		{   
			++x;
			for(int k=0;;k++)					//control random number from to 20 percent
			{ random_ind1=dist1(eng);
				
			if(random_ind1>0 && random_ind1<=(0.3*POPSIZE))
					break;
				else 
					continue;
			}

			for(int k=0;;k++)					//controllig random number to 80 percent
			{ random_ind2=dist1(eng);
					
			if(random_ind2>(0.3*POPSIZE) && random_ind2<(POPSIZE-1))
				break;
			else 
				continue;
			}
		
	

			float a=dist2(eng);
			
			if(a<Pcrossover)
			{
				array_to_vector(pop[i].individual[random_ind1],father);
				array_to_vector(pop[i].individual[random_ind2],mother);
				Crossover(father,mother,son,daughter);
				vector_to_array2(son,seed_individual,pop[i+1].individual[j]);
				vector_to_array2(daughter,seed_individual,pop[i+1].individual[l]);
			}
			else
			{
				pop[i+1].individual[j]=pop[i].individual[random_ind1];
				pop[i+1].individual[l]=pop[i].individual[random_ind2];
			}

				
			father.clear();
			mother.clear();
			son.clear();
			daughter.clear();

			}      */
	
	
		float b=dist2(eng);

		if(b<Pmutation)
		{	
			
			Mutation(pop[i+1]);
		}

		scheduling(pop[i+1],JOBSIZE,seed_individual);

	}

	sorter(pop[GENERATIONS-1]);
	optimal_soln=pop[GENERATIONS-1].individual[0].makespan;

	return optimal_soln;
}


/* void print_fitness(Population &pop,int njobs)
{

fstream outfile("fitness.txt",ios::out | ios::app);
 
	
	if (outfile.is_open())
	{

		for(int i=0;i<POPSIZE;i++)

		{	 	
					outfile<<endl;

					 outfile<<"Makespan for  : "<<(i+1)<<"\t";
					 outfile<<pop.individual[i].makespan;		
		}
	}
	else
		cout<<"Error creating predecessor"<<endl;
		

		system ("pause");

	} */


void open_directory(vector<string> &files, string dirname)
{
	DIR *dir;
	struct dirent *ent;

	/* Open directory stream */
	dir = opendir(dirname.c_str());
	if (dir != NULL)
	{
		// Read the folder and store all the file names inside the "files" list
		while ((ent = readdir(dir)) != NULL)
		{
			if (ent->d_name[0] == '.') continue;
			string filename(ent->d_name);
			files.push_back(filename);
		}
		closedir(dir);
	}
	else
		std::cout << "Error opending folder: " << dirname << endl;
		

}

		


int main()
{ 		
	vector <int> temp1;
	vector <int> temp2;
	vector <int> schedule_set;
	vector <int> eligible_set;
	Population Init_pop;
	Individual seed_individual;
	string line;
	int optimal=0;
	int no_jobs=-1,nrenew=-1,nnonrenew=-1;
	int linecnt=0;
	int nInd=POPSIZE;
	float random;

	string dirname;
	vector<string> files;
	std::cout<<endl;
	std::cout<<"********************************************************************************"<<endl;
	std::cout<<"\t \t RCPSP Scheduling using GENETIC  ALGORITHM"<<endl<<endl;
	std::cout<<"\t \t \t \t \t   - by  SHAIKH SHADAB ANWAR "<<endl<<endl;
	std::cout<<"********************************************************************************"<<endl;
	std::cout<<" Enter name of file to be Solved"<<endl<<endl;
	std::cout << " for example :-"<<"j30, "<<"j60,"<<"j90,"<<"j120 "<<endl<<endl;
	
	while(cin >> dirname)

{
	if(dirname=="j30" || dirname=="j60" || dirname=="j90" || dirname=="j120" || dirname=="j10")
	{
	
	open_directory(files,dirname);

	time_t nowtime;
	ofstream solutionfile;


	solutionfile.open("solution.txt",ios::out|ios::app);

	if(solutionfile.is_open())
	{   
		solutionfile<<endl;
		solutionfile<<"Genetic Algorithm For Solving RCPSP  -by SHAIKH SHADAB ANWAR "<<endl;
		solutionfile<<endl;
		time(&nowtime);
		solutionfile<<"Date/Time    :-  "<<ctime(&nowtime)<<endl;
		solutionfile<<"Crossover Probability   :"<<Pcrossover<<"    "<<"Mutation Probability   :"<<Pmutation<<endl<<endl;
		solutionfile<<"Population Size  :"<<POPSIZE<<"   "<<"Number of Generations  :"<<GENERATIONS<<endl<<endl;
		solutionfile<<"***********************************************************"<<endl<<endl;
	}

	else 
		std::cout<<"error writing file"<<endl;



	for( string filename:files)
	{
		string file= (dirname+ "/" + filename);
		fstream myfile;
		myfile.open(file);
	
	if(myfile.is_open())
	{		
		while (getline(myfile,line))
		{	
		
		//take no of jobs
		
			if (regex_match(line,regex(".*jobs.*incl.*")))
			{	
			smatch s;
			regex_search(line,s,regex("[0-9]+"));
			if (s.size()>0)
				{	no_jobs=stoi(s[0].str());				}
		
			}
			
			//take no of renewable resource
			else if (regex_match(line,regex(".* renewable.*")))
			{	
				smatch s;
				regex_search(line,s,regex("[0-9]+"));
				if (s.size()>0)
				{	nrenew=stoi(s[0].str());
					seed_individual.nrenew=nrenew;	}
				
			}
			
			//take no of non renewable resources
			
			else if (regex_match(line,regex(".*nonrenewable.*")))
			{	
				smatch s;
				regex_search(line,s,regex("[0-9]+"));
				if (s.size()>0)
				{	nnonrenew=stoi(s[0].str()); 
					seed_individual.nnonrenew=nnonrenew; }
			}
			
			
		}

		myfile.close();
	}
	else
		std::cout<<"Error reading No of Jobs, Renewable resources, Non renewable resources"<<endl;
	
	myfile.open(file);

	if(myfile.is_open())
	{  
		int n=0;
		linecnt=0;
		// get the due date

		while(getline(myfile,line))
		{
			++linecnt;
			if(linecnt<15)
				continue;
		
			smatch s;
			regex_match(line,s,regex(".*[0-9]+"));

			stringstream stream (s[0].str());

			stream>>n;
			stream>>n;
			stream>>n;
			stream>>n;
			for(int i=0;i<no_jobs;i++)
			{ 
				seed_individual.job[i].duedate=n;}

			break;
			
		}

		myfile.close();
	}

	else
		std::cout<<"error reading the due date"<<endl;
	
	//get the sucessor

	myfile.open(file);

	if(myfile.is_open())
	{	linecnt=0;
		int i=0;

			while(getline(myfile,line))

			{   int n=0;
				int nsucessor=0;

				++linecnt;
				if(linecnt<19)
					continue;
			
			smatch s;

			regex_search(line,s,regex(".*[0-9]+"));

			stringstream stream(s[0].str());

			stream>>n;	//job count

			seed_individual.job[i].num=n;

			stream>>n; //no of modes

			stream>>n; //no of sucessors

			nsucessor=n;

			seed_individual.job[i].no_sucessor=nsucessor;

			
			for(int j=0;j<nsucessor;j++)
			{
				 stream>>n;
				
				seed_individual.job[i].sucessor.push_back(n);
				
			}

			++i;

			if(i>=no_jobs)
				break;
		}
			myfile.close();
	}
	else
		std::cout<<"error reading the sucessor"<<endl;

	//getting the duration and resource requirement

	myfile.open(file);

	if(myfile.is_open())
	{
	
		int i=0;
		linecnt=0;

		while(getline(myfile,line))
		{ 
			int n=0;
	        ++linecnt;
			if(linecnt<(18+no_jobs+5))
				continue;
			
			smatch s;
			regex_search(line,s,regex(".*[0-9]+"));

			stringstream stream(s[0].str());
		stream>>n;
		stream>>n;
		stream>>n;	//duration

		seed_individual.job[i].duration=n;

		

		for(int j=0;j<nrenew;j++)
		{
			stream>>n;
			seed_individual.job[i].req_renew.push_back(n); //getting resource requirement
		}

		for(int j=0;j<nnonrenew;j++)
		{ 
			stream>>n;
			seed_individual.job[i].req_nonrenew.push_back(n);
		}

		i++;

		if(i>=no_jobs)
			break;
		}

		myfile.close();
	}
	else
		std::cout<<"error reading the duration and resource requirement";


	//getting the total no of renewable and non renewable resources

	myfile.open(file);

	if(myfile.is_open())
	{  
		linecnt=0;
		int n=0;

		while(getline(myfile,line))
		{
			++linecnt;

			if (linecnt<(18+no_jobs+5+no_jobs+3))
				continue;

			smatch s;
			regex_search(line,s,regex(".*[0-9]+"));

			stringstream stream(s[0].str());
			
			for(int i=0;i<nrenew;i++)
			{ 
				stream>>n;
				seed_individual.trenew.push_back(n);
			}

			for(int i=0;i<nnonrenew;i++)
			{ 
				stream>>n;
				seed_individual.tnonrenew.push_back(n);
			}
			break;
		}

		myfile.close();
}
	else
		std::cout<<"Error getting total renewable and nonrenewable resources"<<endl;


//CREATING AN INITIAL POPULATION

	
		//generating the predecessor
	
		for(int i=0;i<no_jobs;i++)
		{   
			for(int j=0;j<no_jobs;j++)
			{
			  for(int k=0;k<(seed_individual.job[j].no_sucessor);k++)
			 {
			
				 if(seed_individual.job[j].sucessor.at(k)==(i+2))
			    	seed_individual.job[i+1].predecessor.push_back(seed_individual.job[j].num); 

		    	else
				 continue;

			}

			}
	}

/*	
 fstream outfile("predecessor.txt",ios::out | ios::app);
 
	

 to print the predecessor

	if (outfile.is_open())
	{

		for(int i=0;i<no_jobs;i++)

		{   
			outfile<<endl;

			outfile<<"The predecessor for  : "<<seed_individual.job[i].num<<"\t";

			for(int j=0;j<((unsigned)seed_individual.job[i].predecessor.size());j++)

			outfile<< seed_individual.job[i].predecessor.at(j) <<" ";

			}
	}

	else
		cout<<"Error creating predecessor"<<endl;
		

		system ("pause");

	}

	*/


	

		// creating schedule set and eligible set


		for(int i=0;i<POPSIZE;i++)
		{
			int temp,b=1;

			//calculating heuristic using convex combination

			for(int j=0;j<no_jobs;j++)
			{
				random=dist(gen);
 				seed_individual.job[j].heuristic=(random*seed_individual.job[j].duedate)+((1-random)*seed_individual.job[j].duration);
				 
			}

			

			Init_pop.individual[i].job[0]=seed_individual.job[0];
			Init_pop.individual[i].job[no_jobs-1]=seed_individual.job[no_jobs-1];

			schedule_set.push_back(1);

			for(int k=0;k<(no_jobs-2);++k)
			{

			
			eligible_set.insert(eligible_set.end(),seed_individual.job[b-1].sucessor.begin(),seed_individual.job[b-1].sucessor.end());

			std::sort(eligible_set.begin(),eligible_set.end());

			eligible_set.erase(unique(eligible_set.begin(),eligible_set.end()),eligible_set.end());

			
			// remove elements if no predecessor in schedule set

			for(vector<int>::iterator it=eligible_set.begin();it!=eligible_set.end();it++)
			{
				std::sort(schedule_set.begin(),schedule_set.end());
				std::sort(seed_individual.job[(*it)-1].predecessor.begin(),seed_individual.job[(*it)-1].predecessor.end());
				
			if(includes(schedule_set.begin(),schedule_set.end(),seed_individual.job[(*it)-1].predecessor.begin(),seed_individual.job[(*it)-1].predecessor.end()))
				continue;	

			else
				temp1.push_back(*it);
							
			}

			for(int n:temp1)
			{  eligible_set.erase(remove(eligible_set.begin(),eligible_set.end(),n),eligible_set.end()); }

			temp1.clear();
		
		

			//arranging the eligible set in ascending order  BUBBLE SORT
	
			for(size_t m=0;m<eligible_set.size();m++)
			{ 
				for(size_t n=0;n<(eligible_set.size()-1);n++)

				{
					if(seed_individual.job[eligible_set.at(n)-1].heuristic>seed_individual.job[eligible_set.at(n+1)-1].heuristic)
					{                                                                                     
																							
						temp=eligible_set.at(n);

						eligible_set.at(n)=eligible_set.at(n+1);					

						eligible_set.at(n+1)=temp;
					}
			}

			}

	
		
			Init_pop.individual[i].job[k+1]=seed_individual.job[eligible_set.front()-1];

				b=eligible_set.front();
				schedule_set.push_back(b);
				std::sort(schedule_set.begin(),schedule_set.end());
				schedule_set.erase(unique(schedule_set.begin(),schedule_set.end()),schedule_set.end());
				eligible_set.erase(eligible_set.begin());
				
			}


			
			schedule_set.clear();
			eligible_set.clear();
		
		}

		cout<<endl;
		std::cout<<endl;
		std::cout<<"Heuristic Calculation Sucessful......"<<endl;	
		
		std::cout<<"Initial Population Sucessfully Generated......."<<endl;

		scheduling(Init_pop,no_jobs,seed_individual);
		
		std::cout<<"Initial Population Sucessfully Scheduled........."<<endl;
		cout<<endl;

		optimal=Solver(Init_pop,seed_individual);

		//print_fitness(Init_pop,no_jobs);
		//std::sort(Init_pop.individual.begin(),Init_pop.individual.end(),[](Individual const &a, Individual const &b)->bool{return (a.makespan<b.makespan);});
		//print_fitness(Init_pop,no_jobs);
		
		std::cout<<"Optimal Solution for :   "<<filename<<"  is  "<<optimal<<endl;
		
		std::cout<<endl<<endl<<"************************************************"<<endl;

		solutionfile<<"Optimal Solution for :    "<<filename<<"\t"<<optimal<<endl;

		for(int l=0;l<JOBSIZE;l++)
		{
			seed_individual.job[l].predecessor.clear();
			seed_individual.job[l].sucessor.clear();
			seed_individual.job[l].req_nonrenew.clear();
			seed_individual.job[l].req_renew.clear();
			seed_individual.tnonrenew.clear();
			seed_individual.trenew.clear();
		}
	

		}
		time_t rawtime;
		time(& rawtime);
		solutionfile<<endl;
		solutionfile<<ctime(& rawtime)<<endl;
		solutionfile.close();
		std::cout<<"Good job"<<endl;
		std::system("pause");

		}
	else
		{	cout<<endl;
			cout<<"Please Enter a Valid Input"<<endl;
			cout<<endl;
			continue;	}

}

		}
		

		

		