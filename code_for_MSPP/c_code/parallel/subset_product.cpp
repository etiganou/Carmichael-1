#include "subset_product.h"
//############################BASIC FUNCTIONS###########################

//FUNCTIONS (1): IMPLAMENTATION OF SIMPLE USEFULL FUNCTONS FOR MPZ OBJECTS

unsigned long long mpz_2_ull(mpz_class z){
	unsigned long long result=0;
	mpz_export(&result,0,-1,sizeof result,0,0,z.get_mpz_t());
	return result;
}

int chrcmp(const char chr1, const char chr2) {
	size_t length1, length2;

	char s1[2] = {chr1, '\0'};
	char s2[2] = {chr2, '\0'};

	length1 = strlen(s1);
	length2 = strlen(s2);

	if (length1 == 1 && length2== 1){
		if (strcmp(s1,s2) == 0){
			return 1;
		}
		else{
			return 0;
		}
	}
	else{
		return 0;
	}
}

void arrcpy(mpz_class* &dest, mpz_class* &source, mpz_class &size){
	for (mpz_class i=0;i<size;i++)
	{	
		dest[mpz_get_ui(i.get_mpz_t())] = source[mpz_get_ui(i.get_mpz_t())];
	}

}

mpz_class fact(mpz_class &n){
	mpz_class f=1;
	for(mpz_class i=n;i>0;i--){
		f*=i;
	}
	return f;	
}

void comb_size(mpz_class &n, mpz_class k, mpz_class &size)
{
	mpz_class tempN,tempK,temp;
	tempN = fact(n);
	tempK = fact(k);
	temp = n-k;
	temp = tempK*fact(temp);
       	size = tempN/temp;	
}


//FUNCTION (1.2): MEMORY CALCULATION FOR UPPER BOUND IN U1

int getmem() {
	struct rusage ru;
	struct rlimit rl;
	getrusage(RUSAGE_SELF, &ru);
	getrlimit(RLIMIT_DATA, &rl);
	unsigned long remaining_memory;
	unsigned long limit = (unsigned long)rl.rlim_cur;
	cout <<"Limit is : " << limit<< endl;
	limit = limit/(unsigned long)1000;
	
	cout <<"Limit is : " << limit<< endl;
	cout <<"Rusage is :" << ru.ru_maxrss << endl; 
	cout <<"Total use :" << float(ru.ru_maxrss)/(float)limit * 100 <<"%"<<endl;
	remaining_memory = limit - (unsigned long)ru.ru_maxrss;	
	return remaining_memory;


}

//FUNCTION (1.3): MAKING BOUND TO DIVIDE HASHMAP
//NEEDS DEVELOPMENT to calcuate on Runtime optimal bound

void U_bound(mpz_class size, mpz_class &bound,int frag){
	
	//frag = 1;		//frag changable parameter 
	//cout <<"fragmentation = " << frag<< endl;
	bound = size/frag;
	return ;

}

//FUNCTION (2): MD5 HASH FUNCTION FOR MPZ_CLASS OBJECTS

string to_md5_f6_str(mpz_class number,char Q_bytes){
	
	string input = number.get_str();
	const char* input_ch_Ar = input.c_str();
       	unsigned char md5digest_ch_Ar[16];		//MD5 digest length == Q_bytes	
	MD5_CTX ctx;
	MD5_Init(&ctx);
	MD5_Update(&ctx, input_ch_Ar, strlen(input_ch_Ar));
	MD5_Final(md5digest_ch_Ar, &ctx);

	char res_ch_Ar[Q_bytes];
	string md5_string;
	for(char i=0;i<Q_bytes;i++)
		sprintf(&res_ch_Ar[i], "%x", (unsigned int)md5digest_ch_Ar[i]);
	
	md5_string.append(res_ch_Ar);
	
	return md5_string;	
}

//FUCNTION (3): IS_CARMICHAEL FUNCTION TO DEAL WITH UNWANTED COLLISIONS

bool is_intersection(mpz_class &Lambda, int* Q_s, int r, unsigned char** &P, mpz_class &u2, int* &E1, int h1, mpz_class* &I, mpz_class &sizeI, mpz_class &c){
	mpz_class* Q;
        Q = new mpz_class[mpz_get_ui(sizeI.get_mpz_t())];
        //cout << "Size of I is : " << mpz_get_ui(sizeI.get_mpz_t()) << endl;
        //cout << "Q1 countains: ";
        for (mpz_class i=0;i<sizeI;i++)
        {
                mpz_class index = I[mpz_get_ui(i.get_mpz_t())];
                Q[mpz_get_ui(i.get_mpz_t())] = get_P_element(Q_s, P[index.get_ui()], r);
        }
	mpz_class temp=func1(Q,E1,Lambda,c,1,h1);
	delete[] Q;
	if (temp==u2)
		return true;
	else
		return false;	
}	

int is_carmichael(mpz_class &n, mpz_class* &factors, mpz_class &sizef)
{
        //cout << "-----DEBUG: ENTERED IS CARMICHAEL-----" << endl;
        int is=1;
        for(mpz_class i=0;i<sizef;++i){
                mpz_class temp;
                mpz_class temp1= n-1;
                mpz_class temp2= factors[mpz_get_ui(i.get_mpz_t())] -1;
                mpz_mod(temp.get_mpz_t(), temp1.get_mpz_t() , temp2.get_mpz_t());
                //cout << "Round " << i << "mod is : " << temp << endl;
                if (temp!=0)
                        is=0;
        }
        return is;
}




//FUCNTION (4): CODE FOR "func1"

mpz_class func1(mpz_class* &P, int* E, mpz_class &Lambda, mpz_class &c, int flag, int size){
		
	if (c<=0)
	{
		cout << "c must be positive" << endl;
		return 0;
	}
	mpz_class prod =1;
	
	if (flag==1){		//element of set U1
				// size is h
		for (mpz_class i=0;i<size;i++){
			//cout << "Prod element: " << P[mpz_get_ui(i.get_mpz_t())] << endl;
			int index = E[mpz_get_ui(i.get_mpz_t())];
			prod *= P[index]; 					//modulo after every prod
			mpz_mod(prod.get_mpz_t(), prod.get_mpz_t(), Lambda.get_mpz_t());
			
		}
	}
	else if (flag==2){		//element of set U2
		
		prod *= c;
		for (mpz_class i=0;i<size;i++) 		//for loop to calculate product
		{
				
				unsigned long long index = E[mpz_get_ui(i.get_mpz_t())];
				//cout << "INDEX IS : " << index << endl;
				mpz_class temp;
				char inverse = mpz_invert(temp.get_mpz_t(), P[index].get_mpz_t(), Lambda.get_mpz_t());

				if(inverse!=0)
				{
					//cout << "Inverse modulo of " << P[mpz_get_ui(i.get_mpz_t())] << " is: " << temp << endl;
					prod *= temp;	
					mpz_mod(prod.get_mpz_t(), prod.get_mpz_t(), Lambda.get_mpz_t());
				}
				else{
					cout << "Inverted is : " << temp << endl;
					//cout << "P[index]=" << P[index].get_mpz_t() << endl;	
					cout << "Number "<< P[index].get_mpz_t() << " doesnt have inverse modulo "<< Lambda << endl;
					//cout << "Pointer is : " << E[mpz_get_ui(i.get_mpz_t())] << endl;
					//cout << "Inverse value is : " << inverse << endl;
					raise(SIGINT);
				}	
	}
	}
	return prod;

}

//FUNCTION (5): CODE FOR "func2"

void func2(mpz_class* &P, int** E, mpz_class &Lambda, mpz_class &c, int flag, unordered_multimap <string,int*> &Map, mpz_class &sizeI, int h1, mpz_class &begin, mpz_class &end, char hash_flag, char Q_bytes){
	
	unsigned long long begin_ull = mpz_2_ull(begin);
	unsigned long long end_ull = mpz_2_ull(end);
	
	
	double func2_time_start = CLOCKTIME();
	#ifdef _OPENMP
		#pragma omp parallel for
	#endif
	for (unsigned long long i=begin_ull;i<end_ull;i++){
		mpz_class temp=func1(P,E[i],Lambda,c,flag,h1);
		//cout <<"Unhashed value: " << temp << endl;
		//cout <<"Hash value: " << to_md5_f6_str(temp) << endl;
		if (hash_flag==1)
		{
			std::pair<string, int*> mypair (to_md5_f6_str(temp, Q_bytes), E[i]);	//IMPLEMENTING HASH			
			#ifdef _OPENMP
				#pragma omp critical
			#endif
                        Map.insert(mypair);
		}
		else
		{
			std::pair<string, int*> mypair (temp.get_str(), E[i]);		//NON-HASH
			#ifdef _OPENMP
				#pragma omp critical
			#endif
                        Map.insert(mypair);
		}				
	}
	
	double func2_time_end = CLOCKTIME();
	
	//cout << "Time for func2 is: " << func2_time_end - func2_time_start << " seconds" << endl;

}

//FUNCTION (6): FUNCTIONS FOR U1

mpz_class get_P_element(int* &Q, unsigned char* &H, int r){
        mpz_class p=1;
        for (int i=0;i<r;i++){
                mpz_class exp;
		mpz_ui_pow_ui(exp.get_mpz_t(), Q[i], H[i]);
                p *= exp;
        }
        return p+1;
}

void U1(int* &Q_s, int r, unsigned char** &P, mpz_class* &I, int** E, mpz_class &Lambda,mpz_class &sizeI,int h1, unordered_multimap<string, int*> &Map, mpz_class &begin, mpz_class &end, char hash_flag, char Q_bytes){

	mpz_class* Q;
	Q = new mpz_class[mpz_get_ui(sizeI.get_mpz_t())];
	//cout << "Size of I is : " << mpz_get_ui(sizeI.get_mpz_t()) << endl;
	//cout << "Q1 countains: ";
	for (mpz_class i=0;i<sizeI;i++)
	{
		mpz_class index = I[mpz_get_ui(i.get_mpz_t())];
		Q[mpz_get_ui(i.get_mpz_t())] = get_P_element(Q_s, P[index.get_ui()], r);
	//	cout << Q[mpz_get_ui(i.get_mpz_t())] << " " ;
	}
	//cout <<"."<<endl;
	cout << endl;
	mpz_class c=1;
	//cout << "ENTERED U1\n";	
	func2(Q,E,Lambda,c,1, Map, sizeI,h1,begin,end,hash_flag,Q_bytes);
	delete[] Q;
}
//FUNCTION (7): FUNCTION FOR SAVING SUBSET AFTER FINDING INTERSECTION

void sol(std::list<int*> &sol1, std::list<int*> &sol2, int* E1, int* E2, int h1, int h2)
{
	//cout << "Entered SOL FUCNTION" << endl;
	int* sol1p = new int[h1];
	int* sol2p = new int[h2];
	for(int i=0;i<h1;i++)
		sol1p[i] = E1[i];
	for(int i=0;i<h2;i++)
		sol2p[i] = E2[i];
	
	sol1.push_back(sol1p);
	sol2.push_back(sol2p);
}

//FUNCTION (8): INTERSECTION FUNCTIONS aka HASHMAP SEARCH

int intersection(int* &Q_s, int r, unsigned char** &P,mpz_class &sizeP, mpz_class** &I, int* E, mpz_class &Lambda, mpz_class &c, mpz_class &sizeE, mpz_class &sizeI, unordered_multimap <string, int*> &Map, mpz_class &count, std::list<int*> &sol1, std::list<int*> &sol2,int h1, int h2, double total_time, char hash_flag, char Q_bytes){
	mpz_class* Q;
	Q = new mpz_class[mpz_get_ui(sizeI.get_mpz_t())];
	//cout << "Q2 contains: ";
	unsigned long long sizeI_ull = mpz_2_ull(sizeI);
	for (unsigned long long i=0;i<sizeI_ull;i++)
	{
		unsigned long long index = mpz_2_ull(I[1][i]);
		Q[i] = get_P_element(Q_s, P[index], r);
		//cout << Q[i] << ", index: " << index <<","; 
	}
	//cout << endl;
        mpz_class temp=func1(Q,E,Lambda,c,2,h2);	
	unordered_multimap<string, int*>::const_iterator got;	
	string key;
	if (hash_flag==0){
		key = temp.get_str();
		got = Map.find(key);
	}
	else{
		key = to_md5_f6_str(temp, Q_bytes);	
		got = Map.find(key);
	}
	//cout << "Hashed number is : " << to_md5_f6_str(temp) << endl;
	if (got != Map.end()){
		auto it = Map.equal_range(key);			//ITERATING ALL MULTI-KEYS
		for (auto itr = it.first; itr != it.second;++itr){
		  	cout << "Key: " << key << endl;
			cout << "In map: " << itr->first << "\t" << endl;

			if(is_intersection(Lambda, Q_s, r,P, temp,itr->second,h1,I[0],sizeI,c))
			{                

			//cout << "Inside intersections"<<endl;		
				unsigned long long sizeP_ull = mpz_2_ull(sizeP);
				cout << endl;
				mpz_class number=1;
				unsigned int del_size = h1 + h2;
       	        		mpz_class* del_set;
               			del_set = new mpz_class[h1+ h2];

               			mpz_class* factors;
               			factors = new mpz_class[sizeP_ull - del_size];
               			for (int k=0;k<h1;k++){
					int index = itr->second[k];
                       			del_set[k] = I[0][index];
                		}
               			for (int k=0;k<h2;k++){
                       			int index = E[k];
                       			del_set[k+h1] = I[1][index];
               			}
			
               			cout << "Variable del_set created" << endl;
               			std::sort(del_set, del_set + del_size);
               			int j=0;
               			mpz_class f_count=0;
               			for (unsigned long long i=0;i<sizeP_ull;i++){
             				if (i != mpz_2_ull(del_set[j])){
                      				number *= get_P_element(Q_s, P[i], r);
						factors[mpz_get_ui(f_count.get_mpz_t())] = get_P_element(Q_s, P[i], r);
                      				++f_count;
                       			}
             				else
                     				++j;
                		}
				delete[] del_set;
				if(is_carmichael(number, factors, f_count) == 1){
					cout << endl << "Found intersection!" <<  endl;
					//cout << "Temp and its hash: " << temp << " : " << to_md5_f6_str(temp, Q_bytes) << endl;
					count++;
					sol(sol1,sol2,itr->second,E, h1, h2); 
			//cout << "RETURNED BEFORE FINISHING SIZE_E"<<endl;
// critical here is because we do not want multiple threads to write the same time to file
				#ifdef _OPENMP
					#pragma omp critical
				#endif
				{
                 		ofstream myfile("carm_num.txt");
                    		myfile << f_count << " : [";
                    		for (unsigned long f=0;f<f_count;f++)
                       			myfile << factors[f] <<", ";
                    		myfile <<"]";
                    		myfile <<"\n";
                    		myfile.close();
							cout << "Carmichael number stored" << endl;
       				delete[] factors;    	
                    		delete[] Q;
							cout << endl<< "Total program time : " <<  CLOCKTIME() - total_time << " seconds" << endl;
                   		 #ifdef _OPENMP
							exit(0);
						 #endif
                		}
               		return 1;
			}
			else
			{
				delete[] factors;    
				cout << "is_carmichael : fasle" << endl;
			}
		}
		else 
			cout << "Found a collision" << endl;
		}
	}
	//printf("\n\n");
	//cout << "!!!!!!!!!!!FOUND " << count << " CARMICHAEL NUMBERS!!!!!!!!!! " << endl;
	//printf ("\n\n\n");
	//cout << "END OF INTERSECTION" << endl; 
	delete[] Q;
	return 0;
}


//FUNCTION (8): GENERATE I SET FUNCTION

void randomize_I(gmp_randclass &rr, unsigned int seed)
{
	rr.seed(seed);
	//rr.seed(time(NULL));
	srand(seed);
}
void gen_I(mpz_class &n, mpz_class &b, int flag, mpz_class** &I, gmp_randclass &rr, unsigned int seed){
        if (b>=n/2){
                cout << "bound must be smaller or equal to the half of P_set size" << endl;
                exit(0);
        }
        mpz_class bound;
        if (flag==0){                   //bound is set to n/2
                cout << "the bound is n/2" << endl;
                bound= n/2;
        }
        else if (flag==1){
                //cout << "the bound is" << b << endl;
                bound = b;
        }
        else{
                cout << "flag can be only 1 or 0" << endl;
                return;
        }

        mpz_class temp;
        mpz_sqrt(temp.get_mpz_t(), n.get_mpz_t());
        if (2*b>temp){
                mpz_class* G;
                G = new mpz_class[mpz_get_ui(n.get_mpz_t())];
                for(mpz_class i=0;i<n;i++)
                        G[mpz_get_ui(i.get_mpz_t())] = i;
                random_shuffle(&G[0], &G[mpz_get_ui(n.get_mpz_t())]);
                for (mpz_class i=0;i<b;i++){
                        mpz_class index = i + b;
                        I[0][mpz_get_ui(i.get_mpz_t())] = G[mpz_get_ui(i.get_mpz_t())];
                        I[1][mpz_get_ui(i.get_mpz_t())] = G[mpz_get_ui(index.get_mpz_t())];
                }
		delete[] G;
        }
        else{
		unordered_map <unsigned int, unsigned int> S;
                mpz_class i=0;
                while(i<2*b)
                {
                        mpz_class choice;
                        //mpz_urandomm(choice.get_mpz_t(), state, n.get_mpz_t());
                        choice = rr.get_z_range(n);
			unordered_map<unsigned int, unsigned int>::const_iterator got = S.find(mpz_get_ui(choice.get_mpz_t()));
                        if (got == S.end())
                        {
                                S[mpz_get_ui(i.get_mpz_t())] = mpz_get_ui(choice.get_mpz_t());
                                i++;
                        }
                }
                for (mpz_class i=0;i<b;i++){
                        mpz_class index = i+b;
                        I[0][mpz_get_ui(i.get_mpz_t())] = S[mpz_get_ui(i.get_mpz_t())];
                        I[1][mpz_get_ui(i.get_mpz_t())] = S[mpz_get_ui(index.get_mpz_t())];
                }
		S.clear();
	}
	
}

//FUNCTION(9): PRODUCT SUBSET ATTACK PHASE 1

int product_attack_1(int* &Q, int r, unsigned char** &P,mpz_class &sizeP, mpz_class &Lambda, mpz_class &c, mpz_class** &I,int local_hamming_weight,mpz_class &sizeI, std::list<int*> &sol1, std::list<int*> &sol2, mpz_class &count, double total_time, int frag, char Q_bytes)
{
	int h1;
	int h2;
	if(local_hamming_weight%2==1){
		h1 = local_hamming_weight/2;
		h2 = h1+1;
	}
	else{
		h1 = local_hamming_weight/2;
		h2=h1;
	}
//	cout << "Probability to found solution: " << success_probability(sizeI, h1, h2);
    	mpz_class sizeE1;
	mpz_class sizeE2;
	comb_size(sizeI, h1, sizeE1);
	comb_size(sizeI, h2, sizeE2);
	
	int** E1;
    	E1 = new int*[sizeE1.get_ui()];
        
	for (mpz_class i=0;i<sizeE1;i++)
                E1[i.get_ui()] = new int[h1];
	//cout << "DEBUG BEFORE PERMS" << endl;
        //-----------COMBINATIONS-------------------//

	Combinations* c_obj1;
	c_obj1	=new Combinations(sizeI.get_ui(),h1);
	
	//cout << "Size of I is : " << sizeI << endl;
	//cout << "h1 = " << h1 << endl;
	//cout << "h2 = " << h2 << endl;
	
	char hash_flag=0;
	if ((Lambda.get_str().length())>Q_bytes)
	{
		hash_flag =1;
		cout << "Hash Implementation: True" << endl;
	}
	else
		cout << "Hash Implementation: False" << endl;
	
	double comb_time_start = CLOCKTIME();
	#ifdef _OPENMP
		#pragma omp parallel for
	#endif
	for (unsigned long i=0;i<sizeE1.get_ui();i++){
		std::vector<int> cmb;	
		#ifdef _OPENMP
			#pragma omp critical
		#endif
		cmb = c_obj1->next_combination();
		int j=0;
		for (std::vector<int>::iterator it = cmb.begin();it != cmb.end(); ++it)
			{
				E1[i][j] = *it;
				j++;
			}
	}
	double comb_time_end = CLOCKTIME();
	//cout << "Combination time: " << comb_time_end-comb_time_start << " seconds" << endl;
	delete c_obj1;
	//cout << "Perms done ! " << endl;
	unordered_multimap <string, int*> U;	
	// cout <<endl;
//	cout <<endl;
//MAKING SLICED U1	
	mpz_class bound;
	
	U_bound(sizeE1,bound,frag);
	int counter=0;
	
	for (;counter<frag;counter++){
		mpz_class begin = counter * bound;
		mpz_class end = (counter+1)*bound;
		if (counter+1!=frag)
			U1(Q, r, P, I[0], E1, Lambda, sizeI,h1, U,begin,end, hash_flag, Q_bytes);
		else
		{	
			begin = sizeE1-bound;
			end = sizeE1;
			U1(Q, r, P, I[0], E1, Lambda, sizeI,h1, U,begin,end, hash_flag, Q_bytes);
		}
		cout << "U1 stored, starting the searching phase..." << endl;
		//cout << "Hashtable size : " << U.size() << endl;
		
//*********************END OF FRAGMENTATION************************************//		
		
		//raise(SIGINT);
		cout << endl;	
		//TESTING THE INSTERSECTION
		Combinations* c_obj2;
        	c_obj2  =new Combinations(sizeI.get_ui(),h2);	
		unsigned long long sizeE2_ull = mpz_2_ull(sizeE2);
		double intersection_time_start = CLOCKTIME();
		#ifdef _OPENMP
			#pragma omp parallel for
		#endif
		for (unsigned long i=0;i<sizeE2_ull;i++){
		    	//cout << "Entered intersection loop" << endl;
			std::vector<int> cmb;
			#ifdef _OPENMP
				#pragma omp critical
			#endif	
            		cmb = c_obj2->next_combination();
           		int j=0;
			int* temp_E = new int[h2];
                	for (std::vector<int>::iterator it = cmb.begin();it != cmb.end(); ++it)
                        {
				temp_E[j] = *it;
                                j++;
                        }
			int inter=0;
			inter = intersection(Q, r, P, sizeP, I, temp_E, Lambda, c, sizeE2, sizeI, U, count,sol1,sol2,h1,h2, total_time, hash_flag, Q_bytes);			
			#ifndef _OPENMP
				if(inter==1){
					break;
				}
			#endif
			delete[] temp_E;
        	}
		
		double intersection_time_end = CLOCKTIME();

		cout << "Intersection time: " << intersection_time_end - intersection_time_start << " seconds" << endl;
		U.clear();
		delete c_obj2;
		//if (inter==1)
		//	break;
	}
	cout << endl;
	for (mpz_class i=0;i<sizeE1;i++)
                delete[] E1[mpz_get_ui(i.get_mpz_t())];

        delete[] E1;
	
	U.clear();
	//cout << endl;
	//cout << " R limit exp: " << endl;
	//long mem = getmem();
	//cout << mem<< endl;
	return 0;	
	
}
