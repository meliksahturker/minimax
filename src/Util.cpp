#include"Util.h"

using namespace std;

void Util::Exit(const char * msg){
		cerr << "[Error] in "<< msg << endl;
		exit(-1);
}



void Util::getTimeStr(char timeStr[]){
		time_t now;
		time(&now);
		strftime(timeStr, 21, "%Y.%m.%d--%H.%M.%S", localtime(&now));
}

string Util::getTimeStr(){
		char time_buf[21];
		time_t now;
		time(&now);
		strftime(time_buf, 21, "%Y.%m.%d--%H.%M.%S", localtime(&now));
		string str(time_buf);
		return str;
}

vector<timeval> Util::ticList;
vector<timeval> Util::tocList;
vector<std::string>  Util::strList;
void Util::tic(string str){
		timeval curTime;
		gettimeofday (&curTime, NULL);
		vector<string>::iterator iter;
		iter = find(strList.begin(), strList.end(), str);
		int idx = iter - strList.begin();
		if (idx==strList.size()){
				strList.push_back(str);
				ticList.push_back(curTime);
				tocList.push_back(curTime);
		}
		ticList[idx]=curTime;
}

double Util::toc(string str){
		timeval curTime;
		gettimeofday (&curTime, NULL);
		vector<string>::iterator iter;
		iter = find(strList.begin(), strList.end(), str);
		int idx = iter - strList.begin();
		if (idx==strList.size()){
				strList.push_back(str);
				ticList.push_back(curTime);
				tocList.push_back(curTime);
		}
		tocList[idx]=curTime;
		double diff = (double (tocList[idx].tv_sec-ticList[idx].tv_sec )
						+ double (tocList[idx].tv_usec-ticList[idx].tv_usec ) *1.e-6 );
		diff =  ceilf(diff *10000)/10000;
		return diff;
}



