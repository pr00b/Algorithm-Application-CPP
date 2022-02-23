#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct Town
{
  int code;   
  string state;  
  string name;   
  int population;  
  float area;  
  float latitude;   
  float longitude;   
  int road_int;   
  float dtoi;        
  Town * new_hash;
};

const int size = 1000000;
Town * table[size];

int Hash(string s)
{ 
  int value = 1294721;    
  for (int i = 0; i < s.length(); i += 1)    
    value = value * 691 + s[i];   
  if (value < 0)        
    value = - value;  
  return value % size; 
}

Town * search(string word, Town * L)
{    
  bool state = false;   
  if (word.size() == 2 && (int)(word[1]) <= 90)   
    state = true;  
  while (L != NULL)
  {        
    string compare = L->name;    
    if (state == true) 
      compare = L->state;    
    if (compare == word)       
      return L;        
    L = L->new_hash;    
  }    
  return NULL;
}

string remove_spaces(const string & s)
{ 
  int last;    
  last = s.size() - 1;  
  while (last >= 0 && s[last] == ' ')        
    --last;    
  return s.substr(0, last + 1);
}

int length(Town * L)
{ 
  int len = 0;  
  while (L != NULL)   
  { 
    len += 1;    
    L = L->new_hash; 
  }  
  return len; 
}

void print_data(Town * L)
{   
  if(L != NULL)
  {        
    cout << L->code << "" << L->state << " " << L->name << " " << L->population << " " << L->area << " " << L->latitude << " " << L->longitude << " " << L->road_int << " " << L->dtoi << "\n";   
    print_data(L = L->new_hash);    
  }
}

int main()
{       
  for (int i = 0; i < size; i += 1)       
    table[i] = NULL;    
  ifstream inf;
  inf.open("/home/www/class/een318/named-places.txt");
  if(inf.fail())
  {
    cout << "Error opening file.\n";
    exit(1);
  }
  else
  {
    string s;   
    while (getline(inf, s))    
    {        
      int code = stoi(s.substr(0, 8));       
      string state = s.substr(8, 2);    
      string name = remove_spaces(s.substr(10, 49));      
      int population = stoi(s.substr(59,11));    
      float area = stof(s.substr(70, 11)); 
      float latitude = stof(s.substr(81, 10));     
      float longitude = stof(s.substr(91, 11));
      int road_int = stoi(s.substr(102, 5));  
      float dtoi = stof(s.substr(107, 7));
      int pos = Hash(name);      
      Town * data = search(name, table[pos]);              
      data = new Town;        
      data->new_hash = table[pos];        
      table[pos] = data;   
      data->code = code;   
      data->name = name;    
      data->state = state;     
      data->population = population;       
      data->area = area;       
      data->latitude = latitude;        
      data->longitude = longitude;       
      data->road_int = road_int;        
      data->dtoi = dtoi;
    }  
    inf.close(); 
    cout << "\nType S placename state to display all information known for the indicated place.\n"
         << "Type N placename to display all information known for all places with the given name in any state.\n"    
         << "Type Q to stop the program.\n\n";
    while (true)
    {
      string input;
      cin >> input;
      string placename;
      string state;
      if(input == "S")
      { 
        cin.ignore();  
        getline(cin,placename);
        string state = placename.substr(placename.length() - 2); 
        string new_placename = placename.erase(placename.length() - 3);
        int pos = Hash(new_placename);       
        int len = length(table[pos]);      
        Town * found = search(new_placename, table[pos]);                          
        Town * found_st = search(state, table[pos]);
        if (found_st != NULL)          
          cout << found_st->code << "" << found_st->state << " " << found_st->name << " " << found_st->population << " "<< found_st->area << " " << found_st->latitude << " " << found_st->longitude << " " << found_st->road_int << " " << found_st-> dtoi << "\n";  
        else            
          cout << "Placename or state not found.\n"; 
      }
      if(input == "N")
      { 
        cin.ignore();
        getline(cin, placename);       
        int pos = Hash(placename);       
        int len = length(table[pos]);      
        Town * found = search(placename, table[pos]);
        if (found != NULL)   
          print_data(table[pos]);        
        else           
          cout << "Placename not found.\n";
      }
      if(input == "Q")
      {
        cout << "Goodbye.\n";
        break;
      }  
    }    
  }
}
