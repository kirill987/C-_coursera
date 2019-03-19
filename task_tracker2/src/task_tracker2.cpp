// Name        : task.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <map>
#include <tuple>
#include <vector>
#include <sstream>

using namespace std;

// Перечислимый тип для статуса задачи
enum class TaskStatus {
	NEW,          // новая
	IN_PROGRESS,  // в разработке
	TESTING,      // на тестировании
	DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

ostream& operator <<(ostream& out, const TaskStatus& status) {
	string statusName;
	switch (status) {
	case TaskStatus::NEW:
		statusName = "\"NEW\"";
		break;
	case TaskStatus::IN_PROGRESS:
		statusName = "\"IN_PROGRESS\"";
		break;
	case TaskStatus::TESTING:
		statusName = "\"TESTING\"";
		break;
	case TaskStatus::DONE:
		statusName = "\"DONE\"";
		break;
	}
	return out << statusName;
}

class TeamTasks {
public:
	// Получить статистику по статусам задач конкретного разработчика
	const TasksInfo& GetPersonTasksInfo(const string& person) const {
		/*
		TasksInfo Tmp_tasks;
		for (int i = 0; i < NUM_OF_KEY + 1; ++i) {
			TaskStatus status = static_cast <TaskStatus> (i);
			if (!Tasks.at(person).at(status)) {
				Tmp_tasks[status] = Tasks.at(person).at(status);
			}
		}*/
		return Tasks.at(person);
		//return Tmp_tasks;
	}
	// Добавить новую задачу (в статусе NEW) для конкретного разработчика
	void AddNewTask(const string& person) {
		++Tasks[person][TaskStatus::NEW];
	}

	// Обновить статусы по данному количеству задач конкретного разработчика,
	// подробности см. ниже
	tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count) {
		TasksInfo untouched; //невыполненные задачи
		TasksInfo updated; //обновившиеся задачи

		if (Tasks.count(person) == 1) { //Если данный человек есть в массиве
			untouched = Tasks[person]; //у него есть невыполненные задачи
		} else {
			return tie (updated, untouched); //иначе возвращаем ничего
		}
		untouched.erase(TaskStatus::DONE); //удаляем из невыполненных задач Done (логично)
		int already_used = 0; // чтобы потом не использовать то, что изменили
//		for (int i = 0; i < NUM_OF_KEY; ++i) { // удаляем строки, если невыполненные задачи равны нулю.
//			TaskStatus cur_status2 = static_cast<TaskStatus>(i);
//			if (untouched[cur_status2] == 0) {
//				untouched.erase(cur_status2);
//			}
//		}
		for (int i = 0; i < NUM_OF_KEY; ++i) { //идём по массиву TaskInfo по ключам
			TaskStatus cur_status = static_cast<TaskStatus>(i); //текущий статус = это i-ый элемент
			TaskStatus next_status = static_cast<TaskStatus>(i+1); //следующий - i+1-ый, выхода за границу нет, т.к. Done не рассматриваем
			//bool checker = false;
			//если кол-во наших задач больше, чем новых, например
			if (untouched.count(cur_status) && task_count >= untouched.at(cur_status) /*- already_used*/) {
				//следующему статусу присвой то, что было в предыдущем, за исключением того, что уже использовалось
				if (untouched[cur_status] /*- already_used */!= 0) {
					updated[next_status] = untouched[cur_status] /* - already_used*/;
				}
				task_count -= untouched[cur_status] /*- already_used */; //уменьши кол-во задач
				untouched.erase(cur_status);
				/*
				if (!already_used) {
					untouched.erase(cur_status); //удали невыполненные задачи от текущего статуса
				} else {
					untouched[cur_status] = already_used;
				} */
				//checker = true;
				Tasks[person][cur_status] = already_used; //already_used; //измени в массиве задач текущий статус данного человека
				Tasks[person][next_status]+=updated[next_status]; //увеличь для человека задачи следующего статуса
				already_used = updated[next_status];
			} else if (untouched.count(cur_status) && task_count < untouched[cur_status] /*- already_used*/) {
				updated[next_status] = task_count;
				//проверка
				untouched[cur_status] -= task_count; //- already_used; //невыполненные от тек статуса
				Tasks[person][cur_status] -= task_count; //- already_used; //текущий статус - вычти все задачи
				Tasks[person][next_status] += updated[next_status];
				already_used = updated[next_status];
				if (untouched[cur_status] == 0) {
					untouched.erase(cur_status);
				}
				break;
			}
			//TaskStatus cur_status = static_cast<TaskStatus>(i);
			if (untouched[cur_status] == 0) {
				untouched.erase(cur_status);
			}
			//cout << "It's me" << untouched.count(cur_status) << endl;
			//cout << untouched.count(cur_status) << endl;
		//cout << already_used << " <-already_used||task_count-> " << task_count << endl;
			//cout << untouched.count(cur_status) << "число невыполненных задач" << endl;
			//cout << untouched[cur_status] << "число невыполненных задач опр статуса" << endl;
		}
		for (int i = 0; i < NUM_OF_KEY; ++i) { // удаляем строки, если невыполненные задачи равны нулю.
			TaskStatus cur_status2 = static_cast<TaskStatus>(i);
			if (untouched[cur_status2] == 0) {
				untouched.erase(cur_status2);
			}
			if (updated[cur_status2] == 0) {
				updated.erase(cur_status2);
			}
		}
	return tie(updated, untouched);
	}
private:
	//объявляем тип, где храним имена людей и их задачи
	map<const string, TasksInfo> Tasks;

	//кол-во статусов задач - 1 (- Done)
	const int NUM_OF_KEY = 3;

};

template <typename Collection>
string Join(const Collection& c, const string& d) {
    stringstream ss;
    bool isFirst = true;
    for (const auto& i : c) {
        if(!isFirst) {
            ss << d;
        }
        isFirst = false;
        ss << i;
    }
    return ss.str();
}
template <typename F, typename S>
ostream& operator <<(ostream& out, const pair<F,S>& p) {
    return out << p.first << ": " << p.second;
}
template <typename K, typename V>
ostream& operator <<(ostream& out, const map<K,V>& m){
    return out << '{' << Join(m, ", ") << '}';
}
template <typename T>
ostream& operator <<(ostream& out, const vector<T>& v) {
   return out << '['  << Join(v, ", ") << ']';
}
int main() {
    TeamTasks tasks;
    while(cin) {
        string command, person;
        int num;
        cin >> command >> person;
        if (command == "AddNewTasks") {
            cin >> num;
            while (num--)
                tasks.AddNewTask(person);
            cout << "[]" << endl;
        } else if (command == "PerformPersonTasks") {
            cin >> num;
            TasksInfo updated_tasks, untouched_tasks;
            tie(updated_tasks, untouched_tasks) =
                    tasks.PerformPersonTasks(person, num);
            cout << vector<TasksInfo>{updated_tasks, untouched_tasks} << endl;
        } else if (command == "GetPersonTasksInfo") {
            cout << tasks.GetPersonTasksInfo(person) << endl;
        } else if (command.size() && command[0] != '#'){
            throw invalid_argument("wrong command: " + command);
        }
    }
}

