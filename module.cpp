#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>

using namespace std;

#include "module.h"

Module::Module() {
    show_menu();
}

void Module::press_any_key_to_continue() {
    std::cout << "Please enter any key to continue... (redirects to the main menu)\n";
    std::system("read");
    show_menu();
}

void Module::show_menu() {
    greeting();
    string str_answer;
    int answer = 0;
    std::cout << "-1.Exit program" <<endl;
    std::cout << "1.Show my GPA" << endl;
    std::cout << "2.Show transfer paths" << endl;
    std::cout << "3.Edit my courses" << endl;
    std::cout << "4.See required courses" << endl;
    getline(cin,str_answer);
        answer = atoi(str_answer.c_str());// used atoi on purpose(no error occurs.) unvalid choice will be converted to zero(0).
    if (answer == -1) {
        std::cout << "Thank you for using our program!\n";
        exit(0);
    }  else if (answer == 1) {
        Module::show_gpa_and_coursework();
    } else if (answer == 2) {
        Module::show_transfer_path();
    } else if (answer == 3) {
        edit_courses();
    } else if (answer == 4) {
        show_remained_courses();
    } else { // if user inputs false value, the string will be automatically converted to 0.
        cout << "Please re-select\n" << endl;
        show_menu();
    }
}

void Module::show_transfer_path() { // If you want to add school, just add line.
    string str_answer;
    int answer;
    show_schools(); // shows schools
    getline(cin,str_answer);
    answer = stoi(str_answer.c_str());
    fstream school_name;
    if (answer == 1) {
        school_name.open("ucberkeley_eecs.txt");
    } else if (answer == 2) {
        school_name.open("ucla_cs.txt");
    } else {
        cout << "Please enter from the options." << endl;
        show_transfer_path();
    }
    show_school_info(school_name);
    school_name.close();
    press_any_key_to_continue();
}

void Module::show_gpa_and_coursework() {
    fstream user;
    user.open("./user.txt");
    string name,school,major;

    int total_credits = 0 ;
    int cumulative_points = 0;
    getline(user,name);
    getline(user,school);
    getline(user,major);

    std::cout << "Hello " << name << ". Your dream school is "<< school <<", " << major << endl;
    std::cout << "You have completed:" << endl;
    int index = 1;
    if (user.eof()) {
        cout << "You did not add any courses. Do you want to add now?\n";
        string choice;
        getline(cin,choice);
        if (choice == "yes") {
            add_courses();
        } else {
            press_any_key_to_continue();
        }
    } else {
        while(!user.eof()){
        string course_number,course_name,credits,grade;
        int point_one_subject,int_credits;
        getline(user,course_number);
        if (course_number == "") {
            break;
        }
        getline(user,course_name);
        getline(user,credits);
        getline(user,grade);
        std::cout << index << ". " << course_number << " " << course_name << " " << credits << " " << grade << endl; 
        point_one_subject = Module::grade_to_number(grade); // change letter grade to number e.g. A --> 4 B --> 3
        int_credits = stoi(credits.c_str());// string to integer.
        total_credits += int_credits; // add credits to total credits
        cumulative_points += (int_credits * point_one_subject); // credit multiply by the grade. e.g. 4 credits course and got an A,  => 4*4 = 16
        ++index ;// plus index
        }                  
    double gpa ;
    gpa = double(cumulative_points) / total_credits;
    std::cout << "Your GPA is " << setprecision(3) << gpa << endl;
    user.close(); 
    press_any_key_to_continue();
    }
}

int Module::grade_to_number(string letter_grade) { // returns integer that corresponds to the given string grade
    int number = 0;
    if (letter_grade == "A") {
        number = 4;
    } else if (letter_grade == "B") {
        number = 3;
    } else if (letter_grade == "C") {
        number = 2;    
    } else if (letter_grade == "D") {
        number = 1;
    } else { //F
        number = 0;
    }
    return number;
}

void Module::edit_courses() {
    string response = "";
    std::cout << "Do you want to add? or delete?" << endl;
    getline(cin,response);
    if ( response == "add" ) {
        Module::add_courses();
    } else if ( response == "delete") {
        Module::delete_courses();
    } else {
        std::cout << "It seems that you entered invalid answer. Please select between two\n";
        Module::edit_courses();
    }
}

void Module::add_courses() {
    fstream user;
    user.open("user.txt",ios::app);
    string course_number,course_name,credits,letter_grade,reply;
    std::cout << "What is the course number you want to add? ex) MATH-294\n";
    getline(cin,course_number);
    std::cout << "What is the name of the course? ex)Differential Equations\n";
    getline(cin,course_name);
    std::cout << "How many credits is your course? ex)5\n";
    getline(cin,credits);
    std::cout << "What did you got from the course? ex)A\n";
    getline(cin,letter_grade);

    std::cout << "You are trying to add " << course_number << " " << course_name << ", "
         << credits << " credits" << " and you got " << letter_grade << "\n";
         // expected output: You are trying to add MATH-294 Differential Equations, 5 credits and you got A"
    std::cout << "Is it correct? (You may want to answer with yes or no)\n";
    getline(cin,reply);
    if (reply == "yes") {
        user << "\n" << course_number << "\n" ;
        user << course_name << "\n" ;
        user << credits << "\n" ;
        user << letter_grade ;
        user.close();
        std::cout << "Do you want to add more courses?\n";
        getline(cin,reply);
        if (reply == "yes") {
            add_courses();
        } else {
            press_any_key_to_continue();
        }

    } else if (reply == "no") {
        std::cout << "Okay, do you want to re-enter your data? or you want to exit?\n";
        string E_or_R;
        getline(cin,E_or_R);
        if (E_or_R == "Re-enter") {
            std::cout << "Okay. please re-enter the information\n";
            add_courses();
        } else  {
            press_any_key_to_continue();
        }

    }

}

void Module::delete_courses() {
    show_my_coursework_only();
    press_any_key_to_continue();
}

void Module::show_my_coursework_only() {
    fstream user;
    user.open("./user.txt");
    string name,school,major;

    getline(user,name);
    getline(user,school);
    getline(user,major);
    vector<string> vec_course_number,vec_course_name,vec_credits,vec_grade;

    int index = 1;
    while(!user.eof()){
        string course_number,course_name,credits,grade;
        getline(user,course_number);
        getline(user,course_name);
        getline(user,credits);
        getline(user,grade);
        if (course_number == "\n" && course_name == "\n" && credits == "\n" &&  grade == "\n") {
            continue; // if the coursework was deleted by the user, it will be changed to newline.
        }
        vec_course_number.push_back(course_number);
        vec_course_name.push_back(course_name);
        vec_credits.push_back(credits);
        vec_grade.push_back(grade);
        std::cout << index << ". " << course_number << " " << course_name << " " << credits << " " << grade << endl;
        ++index;
    }
    string course_to_delete ="";
    int index_course_to_delete = 0;
    cout << "Which course do you want to delete?\n";
    getline(cin,course_to_delete);
    index_course_to_delete = atoi(course_to_delete.c_str()) - 1 ;// because index starts from 1 
    if (index_course_to_delete == -1) {
        cout << "Wrong approach!\n";
        // press_any_key_to_continue();
    } else {
        string reply;
        cout << "You chose " << vec_course_number.at(index_course_to_delete) << "to be deleted. Is this correct?";
        getline(cin,reply);
        cout << reply;
            if (reply == "yes") {
                vec_course_number.erase(vec_course_number.begin()+index_course_to_delete);
                vec_course_name.erase(vec_course_name.begin()+index_course_to_delete);
                vec_credits.erase(vec_credits.begin()+index_course_to_delete);
                vec_grade.erase(vec_grade.begin()+index_course_to_delete);
                user.close();
                fstream user;
                user.open("./user.txt",ios::out | ios::trunc); //delete content and re-write it // reset successful

                user << name << "\n";
                user << school << "\n";
                user << major << "\n";


                for (int i = 0; i < vec_course_name.size() ; ++i ) {
                        user << vec_course_number.at(i) << "\n";
                        user << vec_course_name.at(i) << "\n";
                        user << vec_credits.at(i) << "\n";
                        if ( i == vec_course_name.size()-1) { // I don't want newline at the very last element
                            user << vec_grade.at(i);
                        } else {
                            user << vec_grade.at(i) <<"\n";
                        }             
                    }
                cout << "\ndelete successful" << endl;
                user.close();
            } else {
                press_any_key_to_continue();
            }
        }
    }

void Module::show_remained_courses() {
    vector<vector<string> > schoolinfo(3);
    show_schools();
    string str_choice;
    int choice = 1;
    getline(cin,str_choice);
    choice = atoi(str_choice.c_str());
    if (choice == 0) {
        cout << "Invalid choice! \n";
        press_any_key_to_continue();
    } else if (choice == 1) {
        fstream school_requirement("./ucberkeley_eecs.txt"); // loading berkeley_eecs
        schoolinfo = school_info_into_vector(school_requirement);
        school_requirement.close();
        show_remained_courses_by_schools(schoolinfo);
    } else if (choice == 2) {
        fstream school_requirement("./ucla_cs.txt"); // loading ucla_cs
        schoolinfo = school_info_into_vector(school_requirement);
        school_requirement.close();
        show_remained_courses_by_schools(schoolinfo);
    }
}

void Module::show_remained_courses_by_schools(vector<vector<string> > schoolinfo) {
    vector<vector<string> > myinfo(4);
    vector<vector<string> > not_completed_courses(3);
    fstream myinformation("./user.txt");
    myinfo = my_info_into_vector(myinformation);
    myinformation.close();
        for (int i = 0; i < schoolinfo.at(0).size(); ++i) { // see whether user completed this course.
        int founded = 0;
            for (int j = 0 ; j < myinfo.at(0).size(); ++j) {
                if (schoolinfo.at(0).at(i) == myinfo.at(0).at(j)){ // see if my j th course is in the requirement by searching course number
                    founded = 1;
                } 
            }
                if (founded == 0) { // not founded in trascript means I do not completed that course. Therefore I add it in the course.
                    for (int k = 0 ; k < 3 ; ++k) {
                    not_completed_courses.at(k).push_back(schoolinfo.at(k).at(i)); 
                }
            }
        }
        cout << "You should complete the following--\n";

        for ( int i = 0 ; i < not_completed_courses.at(0).size() ; ++i ) {
            for ( int j = 0 ; j < not_completed_courses.size(); ++j) {
                cout << not_completed_courses.at(j).at(i) << endl;
            }
            cout << endl;
        }
        press_any_key_to_continue();
}

void Module::show_schools() { // change show school
    cout << "Which school and program?" << endl;
    cout << "1.UC Berkeley, EECS" << endl;
    cout << "2.UCLA, Computer Science" << endl;
}

void Module::show_school_info(fstream &schoolinfo) { // used when we want to "show" the requirement of the school
    string name_school,program;
    getline(schoolinfo,name_school);
    getline(schoolinfo,program);
    cout << name_school << ", "<< program << " requires applicants to finish :" << endl;
    while(!schoolinfo.eof()) {
        string course_number,course_name,credits;
        getline(schoolinfo,course_number);
        if(course_number.length() == 0) { // if blank line exits, then exit.
            break;
        }
        getline(schoolinfo,course_name);
        getline(schoolinfo,credits);
        cout << "COURSE NAME: " << course_number << " " << course_name << "| " << credits << " credits" << endl;
    }
}

vector<vector<string> > Module::school_info_into_vector(fstream &schoolinfo) {
        string schoolname, major,temp;
        vector<vector<string> >school_course_info(3); // 0 is for course number, 1 is for course name, 2 is for credit info.

        if(!schoolinfo.is_open()) { // If there's no txt file exits by anyhow, the program exits.
            std::cout << "Unexpected Error. Program exits\n";
            exit(0);
        }

        getline(schoolinfo,schoolname);
        getline(schoolinfo,major);
        while(!schoolinfo.eof()) {
            string course_number,course_name,credits;
            getline(schoolinfo,course_number);
            if(course_number.length() == 0) { // if blank line exits, then exit.
                break;
            }
            getline(schoolinfo,course_name);
            getline(schoolinfo,credits);
            school_course_info.at(0).push_back(course_number);
            school_course_info.at(1).push_back(course_name);
            school_course_info.at(2).push_back(credits);
        }

        schoolinfo.close();
        return school_course_info;
}

vector<vector<string> > Module::my_info_into_vector(fstream &myinfo) {
    vector<vector<string> > my_information(4);
    string name,school,major;
    getline(myinfo,name);
    getline(myinfo,school);
    getline(myinfo,major);

    while(!myinfo.eof()) {
            string course_number,course_name,credits,grades;
            getline(myinfo,course_number);
            if(course_number.length() == 0) { // if blank line exits, then exit.
                break;
            }
            getline(myinfo,course_name);
            getline(myinfo,credits);
            getline(myinfo,grades);
            my_information.at(0).push_back(course_number);
            my_information.at(1).push_back(course_name);
            my_information.at(2).push_back(credits);
            my_information.at(3).push_back(grades);
        }
    return my_information;
}

void Module::not_support() { //I used this for something I will do tmr
    cout << "NOT SUPPORTED YET" << endl;
    press_any_key_to_continue();
}

void Module::successful() { //I used this sometims to debug
    cout << "successful" << endl;
}

string Module::new_user() { // When user.txt file DNE
    ofstream user("user.txt");
    string name,school,major;
    cout << "Hello There!\n";
    cout << "Welcome to transfer tool \n";
    cout << "What's your name?\n--->";
    getline(cin,name);
    cout << "\nwhere do you want to transfer?\n--->";
    getline(cin,school);
    cout << "\nWhat do you want to study there?\n--->";
    getline(cin,major);
    user << name <<"\n" << school << "\n" << major ;
    user.close();
    cout << "Setting completed! ";
    return name;
}

void Module::greeting() {
    string name;
    fstream user;
    user.open("./user.txt");
    getline(user,name);
    if (!user.is_open()) { //"first usage.s"
        name = new_user();
    } 
    user.close();
    cout << "Welcome " << name << "!\n" << endl;
}