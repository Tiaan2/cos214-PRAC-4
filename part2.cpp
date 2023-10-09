#include <iostream>
#include <string>

// Existing ORM using the old grammar
class OldORM {
public:
    std::string executeQuery(const std::string& query) {
        // Simulate executing the query and returning results
        return "Executing old query: " + query;
    }
};

// New database using the new grammar
class NewDatabase {
public:
    std::string executeQuery(const std::string& query) {
        // Simulate executing the query and returning results
        return "Executing new query: " + query;
    }
};

// Adapter to translate between the grammars
class GrammarAdapter {
public:
    GrammarAdapter(OldORM& orm) : orm(orm) {}

    std::string translateQuery(const std::string& query) {
        // Translate the query from the new grammar to the old grammar
        size_t fromPos = query.find("FROM");
        size_t selectPos = query.find("SELECT");
        if (fromPos != std::string::npos && selectPos != std::string::npos && fromPos < selectPos) {
            std::string tableName = query.substr(fromPos + 5, selectPos - fromPos - 6);
            return "SELECT * FROM " + tableName;
        } else {
            return query; // No translation needed
        }
    }

    std::string executeQuery(const std::string& query) {
        std::string translatedQuery = translateQuery(query);
        return orm.executeQuery(translatedQuery);
    }

private:
    OldORM& orm;
};

int main() {
    OldORM oldORM;
    NewDatabase newDatabase;

    // Using the adapter with the old ORM
    GrammarAdapter adapter(oldORM);
    std::string query = "FROM table1 SELECT *";
    std::string result = adapter.executeQuery(query);
    std::cout << result << std::endl;

    // Using the new database directly
    result = newDatabase.executeQuery("SELECT * FROM table1");
    std::cout << result << std::endl;

    return 0;
}
