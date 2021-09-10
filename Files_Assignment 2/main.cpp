#include <iostream>
#include <fstream>
#include <array>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <windows.h>

using namespace std;

vector<string> Words;

WORD GetConsoleTextAttribute (HANDLE hCon)
{
  CONSOLE_SCREEN_BUFFER_INFO con_info;
  GetConsoleScreenBufferInfo(hCon, &con_info);
  return con_info.wAttributes;
}

void Make_Spaces(int len)
{
    for(int i=0; i<25-len;i++)
        cout<<" ";
}
struct Author
{
    char Author_ID[30];    //primary key
    char  Author_Name[50]; //sec. key
    char Author_Address [50];
    char Author_Mobile  [12];

};
struct Book
{
    char Book_ID[13];   //primary key
    char Author_ID_Books[30];  //sec. key
    char Book_Title[50];
    float Book_Price ;

};

struct Condition
{
    string Left ;
    string right;
    string sign;
} ;
int Binary_Search(vector<string> arr, int l, int r, string x)
{
   if (r >= l)
   {
        int mid = l + (r - l)/2;

        if (arr[mid] == x)
        {

             return mid;
        }

        if (arr[mid] > x)
        {

        return Binary_Search(arr, l, mid-1, x);
        }

        return Binary_Search(arr, mid+1, r, x);
   }

   return -1;
}

int Count_Lines(string s)
{
    ifstream lines(s);
    int Lines=0;
     while(!lines.eof())
     {
         string t="";
         getline(lines,t);
         Lines++;
     }
     lines.close();
     return Lines;
}

void Load_Data(string s)
{

  Words.clear();
    ifstream file(s);
     string t;
     while(getline(file,t))
     {
         string name="";
         for(int i = 0; t[i]!='|'; i++)
            name += t[i];
         if(t==""){continue;}
         Words.push_back(name);
     }
}

void Sort_File(string File_Name)
{
    vector<pair<string,string>>K;

    ifstream file(File_Name);


    while(!file.eof())
     {
         string t="";
         string left="",right="";
         getline(file,t);
         if(t=="")continue;
     for(int i = 0; t[i]!='|'; i++)
       {
           left += t[i];
           if(t[i+1]=='|')
            for(int j = i+1; j < t.length(); j++)
                right += t[j];
       }

         K.push_back(make_pair(left,right));
     }
     sort(K.begin(),K.end());
    ofstream fileout(File_Name,ios::out |ios::trunc);

     for(int i=0;i<K.size();i++)
     {
         fileout<<K[i].first<<K[i].second<<endl;

     }

     fileout.close();
     file.close();
}

void Add_New_Book()
{
    Book b;
     ofstream Data_out("Books.txt",ios::out |ios::app);
     ofstream Primary_Key_out("Primary_Key_Books.txt",ios::out |ios::app);
     fstream Secondary("Secondary_Key_Books.txt", ios::in | ios::out);
     fstream List("Label_ID_Books.txt", ios::in | ios::out);
     int Lines = Count_Lines("Books.txt");

        cout<<"\nEnter Book Author ID: ";
        cin.get();
        cin.getline(b.Author_ID_Books,30);
        cout<<"\nEnter Book ID: ";
        cin.getline(b.Book_ID,13);
        cout<<"\nEnter Book Title: ";
        cin.getline(b.Book_Title,50);
        cout<<"\nEnter Book Price: ";
        cin>>b.Book_Price;
        stringstream Price;
        Price<<b.Book_Price;
        int len=strlen(b.Book_ID)+strlen(b.Author_ID_Books)+strlen(b.Book_Title)+4+Price.str().length();
        Data_out<<endl;
        Primary_Key_out<<endl;
        Data_out<<len<<" "<<b.Book_ID<<"|"<<b.Author_ID_Books<<"|"<<b.Book_Title<<"|"<<b.Book_Price<<"|";
        Primary_Key_out<<b.Book_ID<<"|"<<Lines+1;
       cout<<"\nBook Added successfully \n";
       Primary_Key_out.close();
        Data_out.close();


        Sort_File("Primary_Key_Books.txt");

        Load_Data("Secondary_Key_Books.txt");
        int ind = Binary_Search(Words, 0, Words.size()-1, b.Author_ID_Books);
        if(ind == -1)
        {
            int SecLines = Count_Lines("Label_ID_Books.txt");
            Secondary.seekp(0, ios::end);
            Secondary << "\n" << b.Author_ID_Books << "|" << SecLines;
            Secondary.close();
            Sort_File("Secondary_Key_Books.txt");
            List.seekp(0, ios::end);
            List << "\n" << b.Book_ID << "|" << -1;
        }
        else
        {
            int SecLines = Count_Lines("Label_ID_Books.txt");
            string t="";
            for(int i = 0; i < ind; i++)
                getline(Secondary,t);
            getline(Secondary, t, '|');
            int RRNsec,RRNlabel;
            Secondary >> RRNsec;

            for(int i = 0; i < RRNsec; i++)
            {
                getline(List,t);
            }

            getline(List, t, '|');
            List >> RRNlabel;
            if(RRNlabel == -1)
            {

                vector<pair<string,string>>K;
                     List.seekg(0);
                while(!List.eof())
                 {
                     string t="";
                     string left="",right="";

                     getline(List,t);
                     if(t=="")continue;
                 for(int i = 0; t[i]!='|'; i++)
                   {
                       left += t[i];
                       if(t[i+1]=='|')
                        for(int j = i+1; j < t.length(); j++)
                            right += t[j];
                   }

                     K.push_back(make_pair(left,right));
                 }

                 stringstream bridge;
                 bridge << K.size();
                 string temp = "|";
                 temp += bridge.str();
                 K[RRNsec].second=temp;

                         ofstream ata("Label_ID_Books.txt",ios::app);


                 ofstream fileout("Label_ID_Books.txt",ios::out |ios::trunc);

                 for(int i=0;i<K.size();i++)
                 {
                     fileout<<K[i].first<<K[i].second<<endl;

                 }
                 ata<< b.Book_ID <<"|"<<-1<<endl;

                 ata.close();
                fileout.close();


            }
            else if(RRNlabel!=-1)
            {

             int newRRNlabel=RRNlabel;

    while(1)
        {

            List.seekg(0);
            string t="";
            for(int i = 0; i < newRRNlabel; i++)
                getline(List,t);
            getline(List, t, '|');
            List >> newRRNlabel;
            if(newRRNlabel==-1)
            {
                vector<pair<string,string>>K;
                List.seekg(0);
                while(!List.eof())
                 {
                     string t="";
                     string left="",right="";

                     getline(List,t);
                     if(t=="")continue;
                 for(int i = 0; t[i]!='|'; i++)
                   {
                       left += t[i];
                       if(t[i+1]=='|')
                        for(int j = i+1; j < t.length(); j++)
                            right += t[j];
                   }

                     K.push_back(make_pair(left,right));
                 }

                 stringstream bridge;
                 bridge << K.size();
                 string temp = "|";
                 temp += bridge.str();
                 K[RRNlabel].second=temp;

                 ofstream ata("Label_ID_Books.txt",ios::app);
                 ofstream fileout("Label_ID_Books.txt",ios::out |ios::trunc);

                 for(int i=0;i<K.size();i++)
                 {
                     fileout<<K[i].first<<K[i].second<<endl;

                 }
                 ata<< b.Book_ID <<"|"<<-1<<endl;

                 ata.close();
                fileout.close();
                break;

            }
            else if(newRRNlabel!=-1){ RRNlabel=newRRNlabel;}


                }

            }


        }
}
void Add_New_Author()
{
    Author b;
     ofstream Data_out("Authors.txt",ios::out |ios::app);
     ofstream Primary_Key_out("Primary_Key_Authors.txt",ios::out |ios::app);
     fstream Secondary("Secondary_Key_Authors.txt", ios::in | ios::out);
     fstream List("Label_ID_Authors.txt", ios::in | ios::out);

     int Lines=Count_Lines("Authors.txt");

        cout<<"\nEnter Author's ID: ";
        cin.get();
        cin.getline(b.Author_ID,30);
        cout<<"\nEnter Author's Name: ";
        cin.getline(b.Author_Name,50);
        cout<<"\nEnter Author's Address: ";
        cin.getline(b.Author_Address,50);
        cout<<"\nEnter Author's Mobile: ";
        cin.getline(b.Author_Mobile,12);
        int len=strlen(b.Author_Address)+strlen(b.Author_ID)+strlen(b.Author_Name)+strlen(b.Author_Mobile)+4;
        Data_out<<endl;
        Primary_Key_out<<endl;
        Data_out<<len<<" "<<b.Author_ID<<"|"<<b.Author_Name<<"|"<<b.Author_Address<<"|"<<b.Author_Mobile<<"|";
        Primary_Key_out<<b.Author_ID<<"|"<<Lines+1;
  cout<<"\nAuthor Added successfully \n";
        Primary_Key_out.close();
        Data_out.close();

        Sort_File("Primary_Key_Authors.txt");

        Load_Data("Secondary_Key_Authors.txt");

        int ind = Binary_Search(Words, 0, Words.size()-1, b.Author_Name);

        if(ind == -1)
        {
            int SecLines = Count_Lines("Label_ID_Authors.txt");
            Secondary.seekp(0, ios::end);
            Secondary << "\n" << b.Author_Name << "|" << SecLines;
            Secondary.close();
            Sort_File("Secondary_Key_Authors.txt");
            List.seekp(0, ios::end);
            List << "\n" << b.Author_ID << "|" << -1;
        }
        else
        {
            int SecLines = Count_Lines("Label_ID_Authors.txt");
            string t="";
            for(int i = 0; i < ind; i++)
                getline(Secondary,t);
            getline(Secondary, t, '|');
            int RRNsec,RRNlabel;
            Secondary >> RRNsec;

            for(int i = 0; i < RRNsec; i++)
            {
                getline(List,t);
            }

            getline(List, t, '|');
            List >> RRNlabel;
            if(RRNlabel == -1)
            {

                vector<pair<string,string>>K;
                     List.seekg(0);
                while(!List.eof())
                 {
                     string t="";
                     string left="",right="";

                     getline(List,t);
                     if(t=="")continue;
                 for(int i = 0; t[i]!='|'; i++)
                   {
                       left += t[i];
                       if(t[i+1]=='|')
                        for(int j = i+1; j < t.length(); j++)
                            right += t[j];
                   }

                     K.push_back(make_pair(left,right));
                 }

                 stringstream bridge;
                 bridge << K.size();
                 string temp = "|";
                 temp += bridge.str();
                 K[RRNsec].second=temp;

                 ofstream ata("Label_ID_Authors.txt",ios::app);

                 ofstream fileout("Label_ID_Authors.txt",ios::out |ios::trunc);

                 for(int i=0;i<K.size();i++)
                 {
                     fileout<<K[i].first<<K[i].second<<endl;

                 }
                 ata<< b.Author_ID <<"|"<<-1<<endl;

                 ata.close();
                fileout.close();


            }
            else if(RRNlabel!=-1)
            {

             int newRRNlabel=RRNlabel;

    while(1)
        {

            List.seekg(0);
            string t="";
            for(int i = 0; i < newRRNlabel; i++)
                getline(List,t);
            getline(List, t, '|');
            List >> newRRNlabel;
            if(newRRNlabel==-1)
            {
                vector<pair<string,string>>K;
                List.seekg(0);
                while(!List.eof())
                 {
                     string t="";
                     string left="",right="";

                     getline(List,t);
                     if(t=="")continue;
                 for(int i = 0; t[i]!='|'; i++)
                   {
                       left += t[i];
                       if(t[i+1]=='|')
                        for(int j = i+1; j < t.length(); j++)
                            right += t[j];
                   }

                     K.push_back(make_pair(left,right));
                 }

                 stringstream bridge;
                 bridge << K.size();
                 string temp = "|";
                 temp += bridge.str();
                 K[RRNlabel].second=temp;

                 ofstream ata("Label_ID_Authors.txt",ios::app);
                 ofstream fileout("Label_ID_Authors.txt",ios::out |ios::trunc);

                 for(int i=0;i<K.size();i++)
                 {
                     fileout<<K[i].first<<K[i].second<<endl;
                 }
                 ata<< b.Author_ID <<"|"<<-1<<endl;

                 ata.close();
                 fileout.close();
                 break;
            }
            else if(newRRNlabel!=-1){ RRNlabel=newRRNlabel;}
                }

            }


        }

}


string Print_Book_By_BookID(int a,string c)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    const int saved_colors = GetConsoleTextAttribute(hConsole);
    Book b;
    string choice;
    string ID="";
    ifstream Primary("Primary_Key_Books.txt");
    ifstream Data("Books.txt");
    if(a==1){cout<<"\nEnter Book ID : ";cin>>ID;}
    else if(a==2){ID=c;}

    Load_Data("Primary_Key_Books.txt");
    int f=Binary_Search(Words,0,Words.size()-1,ID);
    if(f==-1)cout<<"\nBook is Not Found\n";
    else
    {
       string t="";
            for(int i = 0; i < f; i++)
                getline(Primary,t);
            getline(Primary, t, '|');
            int RRNprim;
            Primary >> RRNprim;
            for(int i = 0; i < RRNprim-1; i++)
                getline(Data,t);
    int len;
    Data>>len;
    Data.getline(b.Book_ID, 13 ,'|');
    Data.getline(b.Author_ID_Books, 30 ,'|');
    Data.getline(b.Book_Title, 50 ,'|');
    Data>>b.Book_Price;
    cout<<"\nBook ID ="<<b.Book_ID<<endl;
    cout<<"Author ID = "<<b.Author_ID_Books<<endl;
    cout<<"Book Title = "<<b.Book_Title<<endl;
    cout<<"Book Price = "<<b.Book_Price<<endl;
    if(a==2){

            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            cout<<"\nis this the Book you want (Y/N) ?\n>>";
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
            cin>>choice;
            SetConsoleTextAttribute(hConsole,saved_colors);
            }

    }
    Primary.close();
    Data.close();

    return choice;


}

string Print_Author_By_AuthorID(int a,string c)
{
    Author b;
    string choice = "";
    string ID="";
    ifstream Primary("Primary_Key_Authors.txt");
    ifstream Data("Authors.txt");

    if(a==1){cout<<"\nEnter Author ID : ";cin>>ID;}
    else if(a==2 || a==3){ID=c;}

    Load_Data("Primary_Key_Authors.txt");
    int f=Binary_Search(Words,0,Words.size()-1,ID);
    if(f==-1 && a!=3)cout<<"\nAuthor is Not Found\n";
    else
    {
       string t="";
            for(int i = 0; i < f; i++)
                getline(Primary,t);
            getline(Primary, t, '|');
            int RRNprim;
            Primary >> RRNprim;
            for(int i = 0; i < RRNprim-1; i++)
                getline(Data,t);
    int len;
    Data>>len;
    Data.getline(b.Author_ID, 30 ,'|');
    Data.getline(b.Author_Name, 50 ,'|');
    Data.getline(b.Author_Address,50,'|');
    Data.getline(b.Author_Mobile,12,'|');
    if(a!=3){
    cout<<"\nAuthor ID ="<<b.Author_ID<<endl;
    cout<<"Author Name = "<<b.Author_Name<<endl;
    cout<<"Author Address = "<<b.Author_Address<<endl;
    cout<<"Author Mobile = "<<b.Author_Mobile<<endl;
    }
    else
    {
        stringstream bridge;
        bridge << b.Author_ID << "|" << b.Author_Name << "|" << b.Author_Address << "|" << b.Author_Mobile << "|";
        choice = bridge.str();
    }
    if(a==2){
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
         const int saved_colors = GetConsoleTextAttribute(hConsole);
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            cout<<"\nis this the Author you want (Y/N) ?\n>>";
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
            cin>>choice;
            SetConsoleTextAttribute(hConsole, saved_colors);

    }


    }
    Primary.close();
    Data.close();
    return choice;
}

void Print_Book_By_AuthorID()
{
    Book b;
    int RRNlabel;
    ifstream Primary("Primary_Key_Books.txt");
    ifstream Secondary("Secondary_Key_Books.txt");
    ifstream List("Label_ID_Books.txt");

    cout<<"\nEnter Author ID :";
    cin>>b.Author_ID_Books;

    Load_Data("Secondary_Key_Books.txt");
    int f=Binary_Search(Words,0,Words.size()-1,b.Author_ID_Books);
    if(f==-1)cout<<"\nThe Author "<<b.Author_ID_Books<<" has no Books \n";
    else
    {
            string t="";
            for(int i = 0; i < f; i++)
                getline(Secondary,t);
            getline(Secondary, t, '|');
            int RRNsec;
            Secondary >> RRNsec;
            for(int i = 0; i < RRNsec; i++)
                getline(List,t);

            List.getline(b.Book_ID,13,'|');

            List>>RRNlabel;

            string choice=Print_Book_By_BookID(2,b.Book_ID);

            if(choice=="Y" |choice=="y"){cout<<"\n Good Choice !!\n";}

            else if(RRNlabel==-1 )
                cout<<"\nSorry,this is the only Book !!\n";
            else if(RRNlabel !=-1)
            {
                while(1)
                {

                  List.seekg(0);
                  for(int i = 0; i < RRNlabel; i++)
                   getline(List,t);

                   List.getline(b.Book_ID,13,'|');

                 List>>RRNlabel;

                choice=Print_Book_By_BookID(2,b.Book_ID);

             if(choice=="Y" |choice=="y"){cout<<"\n Good Choice !!\n";break;}

             if(RRNlabel==-1){cout<<"\nSorry,these are the only Books !!\n";break;}

                }


            }


    }

}


void Print_Author_By_AuthorName()
{
    Author b;
    int RRNlabel;
    ifstream Primary("Primary_Key_Authors.txt");
    ifstream Secondary("Secondary_Key_Authors.txt");
    ifstream List("Label_ID_Authors.txt");

    cout<<"\nEnter Author Name :";
    cin>>b.Author_Name;

    Load_Data("Secondary_Key_Authors.txt");
    int f=Binary_Search(Words,0,Words.size()-1,b.Author_Name);
    if(f==-1)cout<<"\nThere's no Author with the name  "<<b.Author_Name<<" that wrote any books \n";
    else
    {
            string t="";
            for(int i = 0; i < f; i++)
                getline(Secondary,t);
            getline(Secondary, t, '|');
            int RRNsec;
            Secondary >> RRNsec;
            for(int i = 0; i < RRNsec; i++)
                getline(List,t);

            List.getline(b.Author_ID,30,'|');

            List>>RRNlabel;

            string choice=Print_Author_By_AuthorID(2,b.Author_ID);

            if(choice=="Y" |choice=="y"){cout<<"\n DONE !!\n";}

            else if(RRNlabel==-1)
                 cout<<"\nSorry,this is the only Author !!\n";
            else if(RRNlabel !=-1)
            {
                while(1)
                {

                  List.seekg(0);
                  for(int i = 0; i < RRNlabel; i++)
                   getline(List,t);

                   List.getline(b.Author_ID,30,'|');

                 List>>RRNlabel;

             choice=Print_Author_By_AuthorID(2,b.Author_ID);

             if(choice=="Y" |choice=="y"){cout<<"\n DONE !!\n";break;}

             if(RRNlabel==-1){cout<<"\nSorry,these are the only Authors !!\n";break;}

                }


            }


    }

}

void Write_Query()
{
string q,c="",table="",con="";
cout<<"\nWrite A Query: ";
cin.get();
getline(cin,q);
int s=0;
for(int i=0;i<q.length();i++)
    if(q[i]==' ')s++;
int w=s+1;
string Query[w];
for(int i=0;i<w;i++)Query[i]="";
int j=0;
for(int i=0;i<q.length();i++)
{
    if(q[i]==' '){j+=1;continue;}
    Query[j]+=q[i];
}

int f=0;
for(int i=1;i<w;i++)
{
    if(Query[i]=="From"|Query[i]=="from")
        {
    for(int j=i+1;j<w;j++)
        {
    if(Query[j]=="Where"|Query[j]=="where" )
        {
        if(j==w)break;
      for(int b=j+1;b<w;b++)
        {
    if(b==j+1)con=Query[b];
    else con+=" "+Query[b];
         }
        break;
        }
    if(j==i+1)table=Query[j];
    else table+=" "+Query[j];
        }
        break;
        }
    if(i==1)c=Query[i];
    else c+=" "+Query[i];
}


if(c=="all" )
{
    if(table=="Authors and Books" || table=="Books and Authors")
    {
        Condition b;
    istringstream stream(con);
	getline(stream,b.Left,' ');
	getline(stream,b.sign,' ');
	stream>>b.right;

        ifstream file("Books.txt");
         HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  const int saved_colors = GetConsoleTextAttribute(hConsole);

  SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout<<"Author ID"<<"              "<<"Author Name"<<"             "<<"Author Address"<<"             "<<"Author Mobile" << "             ";
        cout<<"Book ID"<<"                 "<<"Book Title"<<"             "<<"Book Price"<<endl;



  SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);

        cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        SetConsoleTextAttribute(hConsole, saved_colors);
        while(!file.eof())
        {
          string t="",temp="";
          bool Print=false;
          getline(file,t);
          if(t=="" || t[2]=='*'){continue;}
          int len=0;

              Book line;
              istringstream sstream(t);
              sstream.seekg(3,ios::beg);
              sstream.getline(line.Book_ID, 13 ,'|');
              sstream.getline(line.Author_ID_Books, 30 ,'|');
              sstream.getline(line.Book_Title, 50 ,'|');
              sstream>>line.Book_Price;

             string Authorinfo = Print_Author_By_AuthorID(3,line.Author_ID_Books);
             Author line2;
              istringstream bstream(Authorinfo);
              bstream.getline(line2.Author_ID, 30 ,'|');
              bstream.getline(line2.Author_Name, 50 ,'|');
              bstream.getline(line2.Author_Address,50,'|');
              bstream.getline(line2.Author_Mobile,12,'|');


              if(b.Left=="Author_Name"){if(b.right==line2.Author_Name)Print=true;}
              if(b.Left=="Author_Address"){if(b.right==line2.Author_Address)Print=true;}
              if(b.Left=="Author_Mobile"){if(b.right==line2.Author_Mobile)Print=true;}
              if(b.Left=="Book_ID"){if(b.right==line.Book_ID)Print=true;}
              if(b.Left=="Author_ID"){if(b.right==line.Author_ID_Books)Print=true;}
              if(b.Left=="Book_Title"){if(b.right==line.Book_Title)Print=true;}
              if(b.Left=="Book_Price")
                {
                    stringstream Price;
                    Price<<line.Book_Price;
                    if(b.right==Price.str())Print=true;
                 }


        if(con=="" || Print==true)
          {
              cout <<  line2.Author_ID ;
              Make_Spaces(strlen(line2.Author_ID));
              cout<<line2.Author_Name ;
              Make_Spaces(strlen(line2.Author_Name));
              cout<<line2.Author_Address ;
              Make_Spaces(strlen(line2.Author_Address));
              cout<< line2.Author_Mobile;
              Make_Spaces(strlen(line2.Author_Mobile));
             cout << line.Book_ID;
             Make_Spaces(strlen(line.Book_ID));
             cout << line.Book_Title ;
             Make_Spaces(strlen(line.Book_Title));
             cout << line.Book_Price << endl;
          }



        }

    }
    else
    {
        Condition b;
    istringstream stream(con);
	getline(stream,b.Left,' ');
	getline(stream,b.sign,' ');
	stream>>b.right;
        string file_name=table+".txt";
        ifstream file(file_name);
         HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
         const int saved_colors = GetConsoleTextAttribute(hConsole);
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

        if(table=="Books")cout<<"\nBook ID"<<setw(23)<<"Author ID"<<setw(26)<<"Book Title"<<setw(25)<<"Book Price"<<endl;
        if(table=="Authors")cout<<"\nAuthor ID"<<setw(24)<<"Author Name"<<setw(24)<<"Author Address"<<setw(27)<<" Author Mobile"<<endl;
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        SetConsoleTextAttribute(hConsole, saved_colors);


        while(!file.eof())
        {
          string t="",temp="";
          bool Print=false;
          getline(file,t);
          if(t=="" || t[2]=='*'){continue;}
          int len=0;
          if(table=="Books")
          {
              Book line;
              istringstream sstream(t);
              sstream.seekg(3,ios::beg);
              sstream.getline(line.Book_ID, 13 ,'|');
              sstream.getline(line.Author_ID_Books, 30 ,'|');
              sstream.getline(line.Book_Title, 50 ,'|');
              sstream>>line.Book_Price;

              if(b.Left=="Book_ID"){if(b.right==line.Book_ID)Print=true;}
              if(b.Left=="Author_ID"){if(b.right==line.Author_ID_Books)Print=true;}
              if(b.Left=="Book_Title"){if(b.right==line.Book_Title)Print=true;}
              if(b.Left=="Book_Price")
                {
                    stringstream Price;
                    Price<<line.Book_Price;
                    if(b.right==Price.str())Print=true;
                 }

          }
          if(table=="Authors")
          {
              Author line;
              istringstream sstream(t);
              sstream.seekg(3,ios::beg);
              sstream.getline(line.Author_ID, 30 ,'|');
              sstream.getline(line.Author_Name, 50 ,'|');
              sstream.getline(line.Author_Address,50,'|');
              sstream.getline(line.Author_Mobile,12,'|');

              if(b.Left=="Author_ID"){if(b.right==line.Author_ID)Print=true;}
              if(b.Left=="Author_Name"){if(b.right==line.Author_Name)Print=true;}
              if(b.Left=="Author_Address"){if(b.right==line.Author_Address)Print=true;}
              if(b.Left=="Author_Mobile"){if(b.right==line.Author_Mobile)Print=true;}
          }
        if(con=="" || Print==true)
          {
        for(int i=3;i<t.length();i++)
          {
              if(i==t.length()-1){break;}
              if(t[i]=='|'){cout<<setw(25-len);len=0;continue;}
              cout<<t[i];
              len++;
          }

          if(table=="Books")cout<<" L.E.";
          cout<<endl<<endl;
          }

        }


    }

}
    else if(c!="all")
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
         const int saved_colors = GetConsoleTextAttribute(hConsole);
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << endl << c << endl;
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout<<"~~~~~~~~~~\n";
        SetConsoleTextAttribute(hConsole, saved_colors);

        Condition b;
        istringstream stream(con);
        getline(stream,b.Left,' ');
        getline(stream,b.sign,' ');
        stream>>b.right;
        string file_name=table+".txt";
        ifstream file(file_name);
            while(!file.eof())
            {
              string t="",temp="";
              bool Print=false;
              getline(file,t);
              if(t==""){continue;}
              int len=0;
              Book line;
              if(table=="Books")
              {
                  istringstream sstream(t);
                  sstream.seekg(3,ios::beg);
                  sstream.getline(line.Book_ID, 13 ,'|');
                  sstream.getline(line.Author_ID_Books, 30 ,'|');
                  sstream.getline(line.Book_Title, 50 ,'|');
                  sstream>>line.Book_Price;

                  if(b.Left=="Book_ID"){if(b.right==line.Book_ID)Print=true;}
                  if(b.Left=="Author_ID"){if(b.right==line.Author_ID_Books)Print=true;}
                  if(b.Left=="Book_Title"){if(b.right==line.Book_Title)Print=true;}
                  if(b.Left=="Book_Price")
                    {
                        stringstream Price;
                        Price<<line.Book_Price;
                        if(b.right==Price.str())Print=true;
                     }

              }
              Author line2;
              if(table=="Authors")
              {

                  istringstream sstream(t);
                  sstream.seekg(3,ios::beg);
                  sstream.getline(line2.Author_ID, 30 ,'|');
                  sstream.getline(line2.Author_Name, 50 ,'|');
                  sstream.getline(line2.Author_Address,50,'|');
                  sstream.getline(line2.Author_Mobile,12,'|');

                  if(b.Left=="Author_ID"){if(b.right==line2.Author_ID)Print=true;}
                  if(b.Left=="Author_Name"){if(b.right==line2.Author_Name)Print=true;}
                  if(b.Left=="Author_Address"){if(b.right==line2.Author_Address)Print=true;}
                  if(b.Left=="Author_Mobile"){if(b.right==line2.Author_Mobile)Print=true;}
              }


            if(con=="" || Print==true)
              {
                  if(table=="Books")
                  {
                      if(c=="Book_ID")cout<<line.Book_ID<<endl;
                      if(c=="Author_ID")cout<<line.Author_ID_Books<<endl;
                      if(c=="Book_Title")cout<<line.Book_Title<<endl;
                      if(c=="Book_Price")cout<<line.Book_Price<<endl;
                  }
                  else if(table=="Authors")
                  {
                      if(c=="Author_ID")cout<<line2.Author_ID<<endl;
                      if(c=="Author_Name")cout<<line2.Author_Name<<endl;
                      if(c=="Author_Address")cout<<line2.Author_Address<<endl;
                      if(c=="Author_Mobile")cout<<line2.Author_Mobile<<endl;
                  }
              }

            }
            }


}

void Delete_Book_By_BookID()
{
    cout << "\nEnter the book ID: ";
    string Book_ID;
    cin >> Book_ID;
    Load_Data("Primary_Key_Books.txt");
    int f=Binary_Search(Words,0,Words.size()-1,Book_ID);
    if(f==-1)
    {
        cout<<"\nBook is Not Found\n";
        return;
    }
    fstream Prim("Primary_Key_Books.txt", ios::in | ios::out);
    string t;
    int tellget = 0;
    for(int i = 0; i < f; i++)
    {
         getline(Prim, t);
         tellget += t.length() + 2;
    }
    Prim.seekp(tellget, ios::beg);
    Prim << "*";
    getline(Prim, t, '|');
    tellget += t.length() + 1;
    cout << "Position in Primary: " << tellget << endl;
    int RRNdata;
    Prim >> RRNdata;
    RRNdata--;
    cout << "RRNData: " << RRNdata << endl;
    Prim.close();
    fstream Data("Books.txt", ios::in | ios::out);
    tellget = 0;
    for(int i = 0; i < RRNdata; i++)
    {
       getline(Data, t);
       tellget += t.length() + 2;
    }
    getline(Data, t, ' ');
    tellget += t.length();
    Data.seekp(tellget, ios::beg);
    Data << "*";
    Data.close();
}

void Delete_Author_By_AuthorID()
{
    cout << "\nEnter the Author ID: ";
    string Author_ID;
    cin >> Author_ID;
    Load_Data("Primary_Key_Authors.txt");
    int f=Binary_Search(Words,0,Words.size()-1,Author_ID);
    if(f==-1)
    {
        cout<<"\nAuthor is Not Found\n";
        return;
    }
    fstream Prim("Primary_Key_Authors.txt", ios::in | ios::out);
    string t;
    int tellget = 0;
    for(int i = 0; i < f; i++)
    {
         getline(Prim, t);
         tellget += t.length() + 2;
    }
    Prim.seekp(tellget, ios::beg);
    Prim << "*";
    getline(Prim, t, '|');
    tellget += t.length() + 1;
    cout << "Position in Primary: " << tellget << endl;
    int RRNdata;
    Prim >> RRNdata;
    RRNdata--;
    cout << "RRNData: " << RRNdata << endl;
    Prim.close();
    fstream Data("Authors.txt", ios::in | ios::out);
    tellget = 0;
    for(int i = 0; i < RRNdata; i++)
    {
       getline(Data, t);
       tellget += t.length() + 2;
    }
    getline(Data, t, ' ');
    tellget += t.length();
    Data.seekp(tellget, ios::beg);
    Data << "*";
    Data.close();
}

int main()
{

        cout<<"\t\t\t   Welcome to the Library";
        int choice;
        while(1)
        {
        cout<<"\n\n1) Add New Book \n2) Add New Author\n3) Delete Book(ID)\n4) Delete Author (ID)\n5) Print Book(ID)  \n6) Print Book(Author_ID)\n7) Print Author(ID) \n8) Print Author(name)\n9) Write a Query \n10) Exit\n\nPlease Enter Your Choice : ";
        cin>>choice;
        if(choice==1)Add_New_Book();
        else if(choice==2)Add_New_Author();
        else if(choice==3)Delete_Book_By_BookID();
        else if(choice==4)Delete_Author_By_AuthorID();
        else if(choice==5)Print_Book_By_BookID(1,"");
        else if(choice==6)Print_Book_By_AuthorID();
        else if(choice==7)Print_Author_By_AuthorID(1,"");
        else if(choice==8)Print_Author_By_AuthorName();
        else if(choice==9)Write_Query();
        else if(choice==10)break;


        }

    return 0;
}
