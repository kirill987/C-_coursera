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

// ������������ ��� ��� ������� ������
enum class TaskStatus {
	NEW,          // �����
	IN_PROGRESS,  // � ����������
	TESTING,      // �� ������������
	DONE          // ���������
};

// ��������� ���-������� ��� map<TaskStatus, int>,
// ������������ ������� ���������� ����� ������� �������
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
	// �������� ���������� �� �������� ����� ����������� ������������
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
	// �������� ����� ������ (� ������� NEW) ��� ����������� ������������
	void AddNewTask(const string& person) {
		++Tasks[person][TaskStatus::NEW];
	}

	// �������� ������� �� ������� ���������� ����� ����������� ������������,
	// ����������� ��. ����
	tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count) {
		TasksInfo untouched; //������������� ������
		TasksInfo updated; //������������ ������

		if (Tasks.count(person) == 1) { //���� ������ ������� ���� � �������
			untouched = Tasks[person]; //� ���� ���� ������������� ������
		} else {
			return tie (updated, untouched); //����� ���������� ������
		}
		untouched.erase(TaskStatus::DONE); //������� �� ������������� ����� Done (�������)
		int already_used = 0; // ����� ����� �� ������������ ��, ��� ��������
//		for (int i = 0; i < NUM_OF_KEY; ++i) { // ������� ������, ���� ������������� ������ ����� ����.
//			TaskStatus cur_status2 = static_cast<TaskStatus>(i);
//			if (untouched[cur_status2] == 0) {
//				untouched.erase(cur_status2);
//			}
//		}
		for (int i = 0; i < NUM_OF_KEY; ++i) { //��� �� ������� TaskInfo �� ������
			TaskStatus cur_status = static_cast<TaskStatus>(i); //������� ������ = ��� i-�� �������
			TaskStatus next_status = static_cast<TaskStatus>(i+1); //��������� - i+1-��, ������ �� ������� ���, �.�. Done �� �������������
			//bool checker = false;
			//���� ���-�� ����� ����� ������, ��� �����, ��������
			if (untouched.count(cur_status) && task_count >= untouched.at(cur_status) /*- already_used*/) {
				//���������� ������� ������� ��, ��� ���� � ����������, �� ����������� ����, ��� ��� ��������������
				if (untouched[cur_status] /*- already_used */!= 0) {
					updated[next_status] = untouched[cur_status] /* - already_used*/;
				}
				task_count -= untouched[cur_status] /*- already_used */; //������� ���-�� �����
				untouched.erase(cur_status);
				/*
				if (!already_used) {
					untouched.erase(cur_status); //����� ������������� ������ �� �������� �������
				} else {
					untouched[cur_status] = already_used;
				} */
				//checker = true;
				Tasks[person][cur_status] = already_used; //already_used; //������ � ������� ����� ������� ������ ������� ��������
				Tasks[person][next_status]+=updated[next_status]; //������� ��� �������� ������ ���������� �������
				already_used = updated[next_status];
			} else if (untouched.count(cur_status) && task_count < untouched[cur_status] /*- already_used*/) {
				updated[next_status] = task_count;
				//��������
				untouched[cur_status] -= task_count; //- already_used; //������������� �� ��� �������
				Tasks[person][cur_status] -= task_count; //- already_used; //������� ������ - ����� ��� ������
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
			//cout << untouched.count(cur_status) << "����� ������������� �����" << endl;
			//cout << untouched[cur_status] << "����� ������������� ����� ��� �������" << endl;
		}
		for (int i = 0; i < NUM_OF_KEY; ++i) { // ������� ������, ���� ������������� ������ ����� ����.
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
	//��������� ���, ��� ������ ����� ����� � �� ������
	map<const string, TasksInfo> Tasks;

	//���-�� �������� ����� - 1 (- Done)
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

