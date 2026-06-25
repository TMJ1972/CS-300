// CS-300 Project Two
//
// ABCU Course Advising Program

// Todd Jarmiolowski

// This is a command driven tool that loads course data from a
// CSV file into a hash table, prints courses in alphanumeric
// order and looks up individual course information
// (including prerequsites) by course number.

// The data structure is a Hash Table with separate chaining
// 0(1) average case lookup for course info (oprion 3)
// Sorting for course list done at print time via vector.
//
//======================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

//======================================================================
// Constants
//======================================================================

const unsigned int DEFAULT_TABLE_SIZE = 179; // Prime number to reduce collisions

//======================================================================
// Structures
//======================================================================

// Holds data for a single course

struct Course {
	string courseNumber;               // e.q. "CSCI100"
	string courseTitle;                // e.g. "Introduction to Computer Science"
	vector<string> prerequisites;      // list of prerequisite course numbers
};

// node for Hash Table chaining

struct Node {
	Course course;
	Node* next;

	// default constructor

	Node() {
		next = nullptr;
	}

	// constructor with a course

	Node(Course aCourse) {
		course = aCourse;
		next = nullptr;
	}
};

//========================================================================
// Hash Table class
//========================================================================

class HashTable {
private:
	vector<Node>nodes;           // vector of bucket head nodes
	unsigned int tableSize;      // number of buckets

	// Hash function. Maps a course number string to a bucket index

	unsigned int Hash(const string& key) {
		unsigned int hash = 0;
		for (char c : key) {
			hash = (hash * 31 + static_cast<unsigned int>(c)) % tableSize;
		}
		return hash;
	}

public:
	// default constructor

	HashTable() {
		tableSize = DEFAULT_TABLE_SIZE;
		nodes.resize(tableSize);
	}
		// constructor with custom size

		HashTable(unsigned int size) {
			tableSize = size;
			nodes.resize(tableSize);
		}

		// destructor - free all dynamically allocated nodes
		~HashTable() {
			for (unsigned int i = 0; i < tableSize; ++i) {
				Node* curr = nodes[i].next;
				while (curr != nullptr) {
					Node* temp = curr;
					curr = curr->next;
					delete temp;
				}
			}
		}

		// insert a course into the hash table

		void insert(Course course) {
			unsigned int key = Hash(course.courseNumber);
			Node* head = &nodes[key];

			// if the head node has no data, store here

			if (head->course.courseNumber.empty()) {
				head->course = course;
			}
			else {
				// walk to the end of the chain and append

				Node* curr = head;
				while (curr->next != nullptr) {
					curr = curr->next;
				}
				curr->next = new Node(course);
			}
		}

		// search for a course by course number, return empty course if not found

		Course Search(string courseNumber) {

			// convert to uppercase fpr case sensitive lookup

			transform(courseNumber.begin(), courseNumber.end(),
				courseNumber.begin(), ::toupper);

			unsigned int key = Hash(courseNumber);
			Node* curr = &nodes[key];

			// walk the chain at this bucket

			while (curr != nullptr) {
				if (curr->course.courseNumber == courseNumber) {
					return curr->course;
				}
				curr = curr->next;
			}

			// not found, return empty course

			Course empty;
			return empty;
		}

		// return all courses as a vector (for sorting and printing)

		vector<Course> GetAllCourses() {
			vector<Course> allCourses;

			for (unsigned int i = 0; i < tableSize; ++i) {
				Node* curr = &nodes[i];

				// only collect if the head bucket has data

				if (!curr->course.courseNumber.empty()) {
					while (curr != nullptr) {
						allCourses.push_back(curr->course);
						curr = curr->next;
					}
				}
			}
			return allCourses;
		}

		// returns true if the table contains at lease on course

		bool IsLoaded() {
			for (unsigned int i = 0; i < tableSize; ++i) {
				if (!nodes[i].course.courseNumber.empty()) {
					return true;
				}
			}
			return false;
		}
	};

	// helper Functions

	// trim leading and trailing whitespace from a string
	
	string Trim(const string& str) {
		size_t start = str.find_first_not_of(" \t\r\n");
		if (start == string::npos) return "";
		size_t end = str.find_last_not_of(" \t\r\n");
		return str.substr(start, end - start + 1);
	}

	// parse one line of CSV and return a vector of fields

	vector<string> ParseCSVLine(const string& line) {

			vector<string> fields;
			stringstream ss(line);
			string field;

			while (getline(ss, field, ',')) {
				fields.push_back(Trim(field));
			}

			return fields;
		}

		// load course data from file into to hash table
		// return line on success, false if the file could not be opened

		bool LoadDataStructure(const string & filename, HashTable & courseTable) {
			ifstream file(filename);

			if (!file.is_open()) {
				cout << "Error: Could not open file \"" << filename << "\"."
					<< "Please check the file name and try again." << endl;
				return false;
			}

			string line;
			int lineNum = 0;
			int loaded = 0;

			while (getline(file, line)) {
				lineNum++;

				// skip blank lines

				if (Trim(line).empty()) {
					continue;
				}

				vector<string> fields = ParseCSVLine(line);

				// A valid course line must have at least course number and title

				if (fields.size() < 2) {
					cout << "Warning: Line " << lineNum
						<< " has less than 2 fields and was skipped." << endl;
					continue;
				}

				// build the course object

				Course course;
				course.courseNumber = fields[0];
				course.courseTitle = fields[1];

				// convert course number to uppercase for consistenicy

				transform(course.courseNumber.begin(), course.courseNumber.end(),
					course.courseNumber.begin(), ::toupper);

				// remaining fields are prerequisite course numbers

				for (size_t i = 2; i < fields.size(); ++i) {
					if (!fields[i].empty()) {
						string prereq = fields[i];
						transform(prereq.begin(), prereq.end(), prereq.begin(), ::toupper);
						course.prerequisites.push_back(prereq);
					}
				}

				courseTable.insert(course);
				loaded++;
				}

				file.close();

				if (loaded == 0) {
					cout << "Warning: No valid course records were found in \""
						<< filename << "\"." << endl;
					return false;
				}

				cout << "Data loaded successfully." << loaded
					<< " course(s) imported." << endl;
				return true;
		}

		// print courses in alphanumeric order

		void PrintCourseList(HashTable & courseTable) {
			if (!courseTable.IsLoaded()) {
				cout << "No data loaded. Please load a data file first (Option 1)." << endl;
				return;
			}


			vector<Course>courses = courseTable.GetAllCourses();

			// set alphanumerically by course number (ascending)

			sort(courses.begin(), courses.end(), [](const Course& a, const Course& b) {
				return a.courseNumber < b.courseNumber;
				});

			cout << "\nHere is a sample schedule.\n" << endl;
			for (const Course& c : courses) {
				cout << c.courseNumber << ", " << c.courseTitle << endl;
			}
			cout << endl;
		}

		// print information for a single course including prerequisite

		void PrintCourse(HashTable & courseTable) {
			if (!courseTable.IsLoaded()) {
				cout << "No data loaded. Please Load A data file first (Option 1)." << endl;
				return;
			}

			string input;
			cout << "What course do you want to know about?";
			cin >> input;

			Course course = courseTable.Search(input);

			if (course.courseNumber.empty()) {
				cout << "Course \"" << input << "\" was not found." << endl;
				return;
			}

			cout << course.courseNumber << "," << course.courseTitle << endl;

			// print prerequisites (or none)

			if (course.prerequisites.empty()) {
				cout << "Prerequisites: None" << endl;
			}
			else {
				cout << "Prerequisites:";
				for (size_t i = 0; i < course.prerequisites.size(); ++i) {
					if (i > 0) cout << ",";

					// look up the prerequisite to print title also (if available)

					Course prereq = courseTable.Search(course.prerequisites[i]);
					if (!prereq.courseNumber.empty()) {
						cout << prereq.courseNumber << "," << prereq.courseTitle;
					}
					else {
						cout << course.prerequisites[i];
					}
				}

				cout << endl;
			}
		}

		// display main menu

		void DisplayMenu() {
			cout << "\n 1. Load Data Structure." << endl;
			cout << " 2. Print Course List." << endl;
			cout << " 3. Print Course." << endl;
			cout << " 9. Exit" << endl;
		}
		//========================================================
		// Main
		//========================================================

		int main() {
			HashTable courseTable;
			int choice = 0;

			cout << "Welcome to the course planner." << endl;

			// Main menu loop

			while (choice != 9) {
				DisplayMenu();

				cout << "\nWhat would you like to do? ";

				// validate the input is actually a number

				if (!(cin >> choice)) {
					cin.clear();
					cin.ignore(1000, '\n');
					cout << "Invalid input. Please enter a number." << endl;
					continue;
				}

				switch (choice) {
				case 1: {
					// load data from file
					string filename;
					cout << "Enter the file name.";
					cin >> filename;
					LoadDataStructure(filename, courseTable);
					break;
				}

				case 2:
					// print sorted course list
					PrintCourseList(courseTable);
					break;

				case 3:
					// print individual course info
					PrintCourse(courseTable);
					break;

				case 9:
					// exit
					cout << "Thank you for using the course planner." << endl;
					break;

				default:
					// invalid menu option
					cout << choice << "is not a valid option." << endl;
					break;

				}
			}
			return 0;
		}


