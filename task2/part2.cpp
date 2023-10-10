#include <iostream>
#include <string>
using namespace std;

// class ORM {  // ADAPTEE
// public:
//     ORM() {
//         query = "SELECT * FROM table";
//     }
    
//     string getQuery() const{
//         return query;
//     }

//     void setQuery(const string& newQuery){
//         query = newQuery;
//     }

// private:
//     string query;   
// };

// class Database {  // TARGET
// public:
//     string executeQuery(const string& query) {
//         return "Executing new query: " + query;
//     }
// };

// // Adapter to translate between the grammars
// class Adapter {  // ADAPTER
// public:
//     Adapter(ORM& orm) : orm(orm) {}

//     void executeQuery() {
//         string query = orm.getQuery();
//         cout << "Translating the query..." << endl;
//         string translatedQuery = translateQuery(query);
//         cout << "Translated query: " << translatedQuery << endl;
//         string result = database.executeQuery(translatedQuery);
//         cout << "Result from the database: " << result << endl;
//         orm.setQuery(result);  // Update the result in the ORM
//     }

//     string translateQuery(string query) {
//         cout << "The query is now getting translated." << endl;
//         return "FROM table SELECT * "; 
//     }

// private:
//     ORM& orm;
//     Database database;
// };

// int main() {
//     ORM orm;
//     Adapter adapter(orm); 
//     adapter.executeQuery(); 
//     cout << "Result stored in ORM: " << orm.getQuery() << endl;

//     return 0;
// }


class Database {  // TARGET
    public:
        virtual ~Database(){};

        virtual string CorrectGrammar() const {
            return "Database: The default target's behavior.";
        }
};

class ORM {  // ADAPTEE
    public:
        ORM(){
            query = "SELECT * FROM table";
        }
        string grammar() const {
            return query;
        }
    private:
        string query;
};

class Adapter : public Database {  // ADAPTER
    private:
        ORM *adaptee_;
    public:
        Adapter(ORM *adaptee) : adaptee_(adaptee) {}
        virtual string CorrectGrammar() const {
            return "ADAPTER: (TRANSLATED)  FROM table SELECT *";
        }
        ~Adapter(){};
};

void Client(Database *target) {  // CLIENT
    cout << target->CorrectGrammar();
}

int main() {
  Database *target = new Database();
  ORM *adaptee = new ORM();
  Adapter *adapter = new Adapter(adaptee);
  cout << "\n";
  cout << "Client: The ORM wants to use this SQL statement:\n";
  cout << "ORM: " << adaptee->grammar();
  cout << "\n\n";
  cout << "Client: The ORM class only supports typical SQL grammar. We however want to use the grammar from our new Database." << endl;
  cout << "Thus we have to translate the ORM grammar.";
  cout << "\n\n";
  cout << "Our Adapter translates the grammar to: \n";
  Client(adapter);
  cout << "\n\n";

  delete target;
  delete adaptee;
  delete adapter;

  return 0;
}

