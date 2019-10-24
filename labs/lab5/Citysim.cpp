#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>
#include <math.h>
#include <stdlib.h>
using namespace std;
class city
{
	public:
		string get_name() { return name; };
		string get_type() { return type; };
		int get_zone() { return zone; };
		float get_latitude() { return latitude; }
		float get_longitude() { return longitude; }
		int get_population() { return population; }
		friend istream &operator>>( istream &, city &);
		friend ostream & operator<<( ostream &, const city &);

	private:
		string name;
		string type;
		int zone;
		float latitude;
		float longitude;
		int population;
  
};

istream & operator>>(istream &in, city &r) { 
	in >> r.zone >> r.name >> r.type >> r.latitude >> r.longitude >> r.population;
	r.latitude = r.latitude*M_PI/180;
	r.longitude = r.longitude*M_PI/180;
}
ostream & operator<<(ostream &out, const city &r) { 
	out << setw(18) << left << r.name << setw(12) << left << r.type << setw(2) << left << r.zone
	<< setw(10) << right << r.population << setw(8) << right << setprecision(2) << r.latitude*180/M_PI
	<< setw(8) << right << setprecision(2) << r.longitude*180/M_PI << endl;
}

class travelcost
{
	public:
		travelcost(vector<city> &v)
		{
			int N = v.size();
			int size = N*(N+1)/2;
			table[0].assign(size, 0.0);
			table[1].assign(size, 0.0);
			for(int i = 0; i < N; i++)
			{
				for(int j = 0; j < i; j++)
				{
					float delta_theta = abs(v[i].get_latitude()-v[j].get_latitude()); //lambda = longitude; theta = latitude
					float delta_lambda = abs(v[i].get_longitude()-v[j].get_longitude());
					float distance = get_distance(delta_lambda, delta_theta, v[i].get_latitude(), v[j].get_latitude());
					distance = 25.0*round(distance/25.0);
					table[0][i*(i+1)/2+j] = distance;
					table[1][i*(i+1)/2+j] = get_time(distance, v[i].get_type(), v[j].get_type());

				}
			}
			cout << "finished with travelcost reading in" << endl;
		}
		float get_distance(float lambda, float theta, float theta1, float theta2)
		{
			//cout << "calculating distance" << endl;
			return 3982*2*asin(sqrt((pow(sin(theta/2), 2))+(cos(theta1)*cos(theta2)*pow(sin(lambda/2),2))));
		}
		float get_time(float distance, string type1, string type2)
		{
			if((type1.compare("GATEWAY") == 0) || (type2.compare("GATEWAY") == 0))
			{
				return distance/520.0;
			}
			return distance/65.0;
		}
		float operator()(int mode, int i, int j)
		{
			if(i < j)
			{
				int tmp = i;
				i = j;
				j = tmp;
			}
			return table[mode][i*(i+1)/2+j];
		}
	private:
		vector<float> table[2];
};

void read_cityinfo(vector<city> &v) {
	ifstream fin;
	fin.open("city_list.txt");
	string line;
	while(getline(fin, line))
	{
		city tmp;
		//cout << line << endl;
		if((!line.empty()) && line.at(0) != '#')
		{
			stringstream ss(line);
			ss >> tmp;
			v.push_back(tmp);
			//cout << count++ << endl;
		}
		//cout << count++ << endl;
	}
	fin.close();
}
void write_traveldistance(vector<city> &v, travelcost &tc)
{
	ofstream fout;
	fout.open("city_distancetable.txt");
	int N = v.size();
	int size = N*(N+1)/2;
	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < i; j++)
		{
			float distance = tc(0, i, j);
			fout << i << " " << setw(38) << left << v[i].get_name() + " to " + v[j].get_name() << " " << setfill('.') << " " << right << distance << " miles " << endl;
		}
	}

	fout.close();
}
void write_traveltime(vector<city> &v, travelcost &tc)
{
	ofstream fout;
	fout.open("city_timetable.txt");
	int N = v.size();
	int size = N*(N+1)/2;
	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < i; j++)
		{
			float time = tc(1, i, j);
			fout << i << " " << setw(38) << left << v[i].get_name() + " to " + v[j].get_name() << " " << setfill('.') << " " << right << setprecision(3) << time << " hours" << endl;
		}
	}

	fout.close();
}

void write_cityinfo(vector<city> &v)
{
	ofstream fout;
	fout.open("city_info.txt");
	for(vector<city>:: iterator it = v.begin(); it != v.end(); ++it)
	{
		fout << *it;
	}

	fout.close();

}
void create_citygraph(vector<city> &v, travelcost &tc, vector<vector<int> > &graph) 
{
	vector<int> regional;
	vector<int> gateway;
	for(int i = 0; i < v.size(); i++)
	{
		if(v[i].get_type().compare("REGIONAL") == 0)
		{
			regional.push_back(i);
		} else
		{
			gateway.push_back(i);
		}
	}
	graph.assign(v.size(),vector<int>());
	for(int i = 0; i < graph.size(); i++)
	{
			int v_zone = v[i].get_zone();
			string v_type = v[i].get_type();
			int prev_zone = -1;
			if(v_type.compare("REGIONAL") == 0)
			{
				float min_dist = 1000000;
				int min_index = -1;
				for(vector<int>::iterator it = regional.begin(); it != regional.end(); it++)
				{
					if(i == *it)
					{
						continue;
					}
					int r_zone = v[*it].get_zone();
					if(v_zone == r_zone)
					{
						graph[i].push_back(*it);
						graph[*it].push_back(i);
					}
					for(vector<int>::iterator g_it = gateway.begin(); g_it != gateway.end(); g_it++)
					{
						int g_zone = v[*g_it].get_zone();
						if(g_zone == v_zone)
						{
							float dist = tc(0, i, *g_it);
							if((g_it != gateway.begin()) && ((dist < min_dist)))
							{
							
								min_dist = dist;
								min_index = *g_it;
							}	
							else if ((g_it == gateway.begin()))
							{
								min_dist = dist;
								min_index = *g_it;
							}
						} else if(prev_zone == v_zone)
						{
							graph[i].push_back(min_index);
							graph[min_index].push_back(i);
								min_dist = 1000000;
								min_index = -1;
						}
						prev_zone = g_zone;
					}

				}
			} 
			else if (v_type.compare("GATEWAY") == 0)
			{
				float min_dist = 1000000;
				int min_index = -1;
				for(vector<int>::iterator it = gateway.begin(); it != gateway.end(); it++)
				{
					if(i == *it)
					{
						continue;
					}
					int g_zone = v[*it].get_zone();
					float dist = tc(0, i, *it);
					if(v_zone == g_zone)		//same zone
					{
						if(min_index != -1)
						{
						graph[i].push_back(min_index);
						graph[min_index].push_back(i);
						min_dist = 1000000;
						min_index = -1;
						}
						graph[i].push_back(*it);
						graph[*it].push_back(i);
					} 
					else if((it != gateway.begin()) && (prev_zone == g_zone)) //not at Cairo and in different zone now
					{
						if((dist <= 6000.0) && (dist < min_dist))
						{
							min_dist = dist;
							min_index = *it;
						}
					}
					else if ((it != gateway.begin()) && min_index != -1) //there was a min distance in previous zone
					{
						graph[i].push_back(min_index);
						graph[min_index].push_back(i);
						if(dist <= 6000)
						{
							min_dist = dist;
							min_index = *it;
						} else {
						min_dist = 1000000;
						min_index = -1;
						}
					}
					else if ((it != gateway.begin()) && min_index == -1) //there were no distanecs <6000 in previous zone
					{
						if(dist <= 6000)
						{
							min_dist = dist;
							min_index = *it;
						}
					}
					else if ((it == gateway.begin()) && (dist <= 6000)) //at cairo and is less than 6000
					{
						min_dist = dist;
						min_index = *it;
						if (it++ == gateway.end())
						{
							graph[i].push_back(min_index);
							graph[min_index].push_back(i);
							min_dist = 1000000;
						min_index = -1;
						}
					}
					prev_zone = g_zone;

				}
				if(min_index != -1)
						{
						graph[i].push_back(min_index);
						graph[min_index].push_back(i);
						min_dist = 1000000;
						min_index = -1;
						}
			}

	}
	for(int i = 0; i < graph.size(); i++)
	{
		sort( graph[i].begin(), graph[i].end() );
		graph[i].erase( unique( graph[i].begin(), graph[i].end() ), graph[i].end() );
	}
}
void write_citygraph(vector<city> &v, travelcost &tc, vector<vector<int> > &graph)
{
	ofstream fout;
	fout.open("city_graph.txt");
	cout << "CITY GRAPH: \n";
	for(int i = 0; i < graph.size(); i++)
	{
		cout << endl;
		cout << setw(3) << right << i << " " << v[i].get_name() << endl;

		for(int j = 0; j < graph[i].size(); j++)
		{
			int id = graph[i][j];

			cout << setw(6) << right << id << " " << setw(18) << left << v[id].get_name() << " " << fixed << setprecision(1)
			<< setw(7) << right << tc(0, i, id) << " miles " << setw(5) << right << tc(1, i, id) << " hours" << endl;
		}
	}
}
//dijkstra_route() { }

int main(int argc, char *argv[])
{
  //option decoding
	string usage = "usage: ./Citysim -distance|time|graphinfo";
  	if (argc != 2){
		cerr << usage << endl;
		return 1;
	}
	
	if(string(argv[0]) != "./Citysim"){
		cerr << usage << endl;
		return 1;
	}

	if(string(argv[1]) != "-graphinfo"){
		cerr << usage << endl;
		return 1;
	}

  //object declarations

  vector<city> list;
  read_cityinfo(list);
  write_cityinfo(list);
	//cout << "on to travelcosts" << endl;
  travelcost tc(list);
  //cout << "writing travel distance" << endl;
	write_traveldistance(list, tc);
	//cout << "writing traveltime" << endl;
	write_traveltime(list, tc);

  //set up travelcosts
	vector<vector<int> > graph;
    create_citygraph(list, tc, graph);
	write_citygraph(list, tc, graph);

  //while (not done) {
	//ask for form, to cities
    //dijkstra_route(from,to) 
  //}
}
