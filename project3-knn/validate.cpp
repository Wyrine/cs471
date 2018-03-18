#include "validate.hpp"
#include <cctype>

using namespace std;

Validation::Validation(const char* data, const char* grp, const uint _m, 
				const uint _f, const uint _c)
{
		classes = _c;
		features = _f;
		thread t1(&Validation::readGroupingFile, this, grp);
		thread t2(&Validation::readAndBuildMatrix, this, data);
		t1.join(); t2.join();
}
		void
Validation::Validate(const uint m)
{


}
		void
Validation::readGroupingFile(const char* grp)
{
		char junk;
		vector<short> curLine;
		short curVal;
		ifstream input(grp);
		if(! input.is_open() )
		{
				cerr << "Failed to open " << grp << ". Exiting.";
				exit(1);
		}

		do{
				do{
						input >> curVal;
						//decrement to use it as index
						curLine.push_back(curVal - 1);
				}while( isspace(input.peek()) );
				groups.push_back(curLine);
				curLine.clear();
		}while( input >> junk );
		input.close();
}
		void
Validation::readAndBuildMatrix(const char* data)
{
		ifstream input(data);
		string line;
		double tmp[features + 1];
		int i;
		if(! input.is_open() )
		{
				cerr << "Failed to open " << data << ". Exiting.";
				exit(1);
		}
		getline(input, line);
		while( getSamp(input, tmp) )
		{
				Sample s(features, tmp);
				dataSet.push_back(s);
		}
		input.close();	
}
		double
Validation::mComp(const int _cur)
{
		return (_cur > 3) ? _cur - 2 : _cur - 1;
}
		bool
Validation::getSamp(ifstream & input, double samp[])
{
		int i; string tmp;

		for(i = 0; i < features && (input >> samp[i]); i++);
		if(i < features) return false;
		
		if(compFunc == nullptr)
		{
				input >> samp[i];
				samp[i] = mComp(samp[i]);
		}
		else
		{
				input >> tmp;
				samp[i] = compFunc(tmp);
		}
		return true;
}
