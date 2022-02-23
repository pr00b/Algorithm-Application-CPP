#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct Person
{
  int ssn;
  int dob;
  string fname;
  string lname;
  int zip;
};

struct Node
{
  Person data;
  Node * left; 
  Node * right;

  Node(int ssn = NULL, int dob = NULL, string f = NULL, string l = NULL, int z = NULL, Node *ln = NULL, Node *rn = NULL)
  {
    data.ssn = ssn;
    data.dob = dob;
    data.fname = f;
    data.lname = l;
    data.zip = z;
    left = ln;
    right = rn;
  }
};

class BTree
{
  protected:
    Node * root;
  public:
    BTree()
    {
      root = NULL;
    }

    void insert_to_tree(Node * &T, int ssn, int dob, string f, string l, int z)
    {
      if (T == NULL)
      {
        T = new Node(ssn, dob, f, l, z);
          return;
      }
        if (f + l < T->data.fname + T->data.lname)
          insert_to_tree(T->left, ssn, dob, f, l, z);
        else
          insert_to_tree(T->right, ssn, dob, f, l, z);
    }

    void insert_loop(Node * &T, int ssn, int dob, string f, string l, int z)
    {
      if (T == NULL)
        {
          T = new Node(ssn, dob, f, l, z);
            return;
        }
      Node * prev = NULL;
      while (T != NULL)
      {
        prev = T;
        if (f + l < T->data.fname + T->data.lname)
          T = T->left;
        else
          T = T->right;
        }
        if (f + l < T->data.fname + T->data.lname)
          prev->left = new Node(ssn, dob, f, l, z);
        else
          prev->right = new Node(ssn, dob, f, l, z);
    }

    void add_to_tree(int ssn, int dob, string f, string l, int z)
    {
      insert_to_tree(root, ssn, dob, f, l, z);
    }
	
    void print(Node * D)
    {
      if (D == NULL)
        return;
      print(D->left);
      cout << D->data.ssn << " " << D->data.dob << " " << D->data.fname << " " << D->data.lname << " " << D->data.zip << "\n";
      print(D->right);
	}

    void get_zip(Node * D, int z)
    {
      if (D == NULL)
        return;
      get_zip(D->left, z);
      if (z == D->data.zip)
        cout << D->data.fname << " " << D->data.lname << "\n";
      get_zip(D->right, z);
    }

    void get_oldest(Node * D, Person &data)
    {
      if (D == NULL)
        return;
      get_oldest(D->left, data);
      if (data.dob > D->data.dob)
      {
        data.fname = D->data.fname;
        data.lname = D->data.lname;
        data.ssn = D->data.ssn;
        data.dob = D->data.dob;
        data.zip = D->data.zip;
      }
      get_oldest(D->right, data);
    }

    bool find(string fn, string ln)
    {
      Node * C = root;
      while (C != NULL)
      {
        if (C->data.fname + C->data.lname  == fn + ln)
        {
          cout << C->data.ssn << " " << C->data.dob << " " << C->data.fname << " " << C->data.lname << " " << C->data.zip << "\n";
          return true;
        }
        else if (fn + ln < C->data.fname + C->data.lname)
          C = C->left;
        else
          C = C->right;
        }
      cout << "This person cannot be found.\n";
      return false;
    }
   
    void print()
    {
      print(root);
    }

    void zip(int zip)
    {
      get_zip(root, zip);
    }

    void oldest()
    {
      if(root == NULL) 
        return;
      Person data;
      data.zip = root->data.zip;
      data.dob = root->data.dob;
      data.ssn = root->data.ssn;
      data.fname = root->data.fname;
      data.lname = root->data.lname;
      get_oldest(root, data);
      cout << data.fname << " " << data.lname << " " << data.zip << "\n";
    }

    Node * find_min(Node * & T)
    {
      Node * curr = T;
      while(curr->left != NULL)
        curr = curr->left;
      return curr;
    }

    void delete_person(string fn, string ln, Node * & X)
    {
      if(X == NULL)
      {
        cout << "This person has already been deleted or never existed.\n";
        return;
      }
      else if(fn + ln < X->data.fname + X->data.lname)
        delete_person(fn, ln, X->left);
      else if(fn + ln > X->data.fname + X->data.lname)
        delete_person(fn, ln, X->right);
      else
        {
          if (X->left == NULL && X->right == NULL)
            X = NULL;
          else if(X->left == NULL)
            X = X->right;
          else if(X->right == NULL)
            X = X->left;
          else
          {
            Node * temp = find_min(X->right);
            X->data = temp->data;
          }
       cout << "Person deleted.\n";
        }
    }

    void delete_now(string firstn, string lastn)
    {
      delete_person(firstn, lastn, root);
    }
};

BTree obj;

int main()
{  
  ifstream inf;
  inf.open("database.txt");
  if (inf.fail())
  {
    cout << "Error opening file\n";
    exit(1);
  }
  else
  {  
    while (true)
    {
      int ssn;
      int dob; 
      int zip;
      string fname; 
      string lname;
	  inf >> ssn >> dob >> fname >> lname >> zip;
	  obj.add_to_tree(ssn, dob, fname, lname, zip);
      if (inf.fail())
        break;
    }
  inf.close();
  }

  cout << "\nBinary Tree Database Menu:\n\n" 
       << "Type FIND firstname lastname to display all information about the named person.\n"
       << "Type PRINT to display all information about everyone in the database, in alphabetical order based on their names.\n"
       << "Type ZIP zipcode to display the names of all people living in the given zip code.\n"
       << "Type OLDEST to print the name and zipcode of the oldest person in the database.\n"
       << "Type DELETE firstname lastname to remove the indicated entry from the list.\n"
       << "Type EXIT to exit from the program.\n\n";

  while (true)
  {
    string input;
    cin >> input;
    if (input == "FIND")
    {
      string fname;
      cin >> fname;
      string lname;
	  cin >> lname;
      obj.find(fname, lname);
    }
	if (input == "PRINT")
	{
	  obj.print();
	}
	if (input == "ZIP")
	{
	  int z;
	  cin >> z;
	  obj.zip(z);
	}
    if (input == "OLDEST")
      {
      obj.oldest();
      }
    if (input == "DELETE")
    {
      string fname;
      cin >> fname;
      string lname;
	  cin >> lname;
      obj.delete_now(fname, lname);
    }
    if(input == "EXIT")
      break;
  }
}
