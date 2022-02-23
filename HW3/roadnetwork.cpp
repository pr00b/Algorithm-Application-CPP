#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Connection
{
  public:
    int begin;
    int end;
    float distance;
    string placename;
    string type;

    Connection(string pn, string t, int b, int e, float d)
    {
      placename = pn;
      type = t;
      begin = b;
      end = e;
      distance = d;
    };
};

class Location
{
  public:
    int index;
    string state;
    string name;
    float longitude;
    float latitude;
    float distance;
    vector<Connection> cnxn;
  
    Location(int i, float lo, float la, float d, string s, string n)
    {
      index = i;
      state = s;
      name = n;
      longitude = lo;
      latitude = la;
      distance = d;
    };

    void add_connection(Connection add)
    {
      cnxn.push_back(add);
    };
};

int main()
{
  int begin;
  int end;
  int index = 0;
  double longitude;
  double latitude;
  double distance;
  string state;
  string name;
  string type;
  vector<Location> L;
  ifstream inf_intxn("/home/www/class/een318/intersections.txt");

  while(inf_intxn >> longitude >> latitude >> distance >> state)
  {
    getline(inf_intxn, name);
    Location location(index, longitude, latitude, distance, state, name);
    L.push_back(location);
    index+=1;
  }
  inf_intxn.close();
  ifstream inf_cnxn("/home/www/class/een318/connections.txt");

  while(inf_cnxn >> name >> type >> begin >> end >> distance)
  {
    Connection conn1(name, type, begin, end, distance);
    Connection conn2(name, type, end, begin, distance);
    L[begin].add_connection(conn1);
    L[end].add_connection(conn2);
  }
  inf_cnxn.close();
  cout << "Location to start (Must be a number from 1 to "<< L.size() - 1 << "):\n";
  cin >> index;
  if(index < 0 || index >= L.size())
  {
    cerr << "\nError, closing program.\n";
    return 0;
  }

  while(true)
  {
    vector<Connection> C = L[index].cnxn;
    cout << "Location " << L[index].index << ", " << L[index].distance
         << " miles from" << L[index].name << ", " << L[index].state << "\n"
         << "roads leading away: \n";
    for(int i = 0; i < C.size(); i+=1)
    {
      cout << " " << i + 1 << ": " << C[i].placename << ", " << C[i].distance
           << " miles to location " << C[i].end << endl;
    }
    int connection_index = 0;
    while(connection_index < 1 || connection_index > C.size())
    {
      cout << "Take which road? ";
      cin >> connection_index;
      if (connection_index > C.size())
      {
        cerr << "Error, Selection out of range. Closing program.\n";
        return 0;
      }
      if (connection_index == -1)
      {
        cout << "Exiting.\n";
        break;
      }
      cout << "\n";
    }
  index = C[connection_index - 1].end;  
  }
}
