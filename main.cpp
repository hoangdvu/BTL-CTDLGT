#include <iostream>
#include <string>

using namespace std;

struct SELEM {
    string time;
    string plate;
    string data;
    SELEM* next;
};

typedef SELEM ELEM, * PELEM, * LLIST;

void Print(const LLIST& l) {
    for (LLIST temp = l; temp; temp = temp->next) {
        cout << temp->plate << endl;
    }
}

LLIST llInit() {
    return NULL;
}

LLIST llInsertTail(LLIST l, const string& data, const string& time = "", const string& plate = "") {
    PELEM e = new ELEM;
    e->data = data;
    e->time = time;
    e->plate = plate;
    e->next = NULL;

    if (!l) return e;

    PELEM p = l;
    while (p->next) {
        p = p->next;
    }
    p->next = e;
    return l;
}

int Find(LLIST l, const string& plate) {
    int i = 1;
    for (; l; l = l->next, ++i) {
        if (l->plate == plate) {
            return i;
        }
    }
    return -1;
}

LLIST llDelete(LLIST l, PELEM a) {
    if (!a) return l;

    if (l == a) {
        LLIST temp = l->next;
        delete l;
        return temp;
    }

    PELEM current = l;
    while (current->next && current->next != a) {
        current = current->next;
    }

    if (current->next == a) {
        current->next = a->next;
        delete a;
    }

    return l;
}

PELEM llSeek(LLIST l, int i) {
    for (; i > 1 && l; --i) {
        l = l->next;
    }
    return l;
}

int Check(PELEM l) {
    int dem = 0;
    for (int i = 0; i < 4 && i < l->plate.size(); i++) {
        if (l->plate[i] == 'x') {
            dem++;
        }
    }
    return (dem == 4) ? 1 : 0;
}

int Bill(LLIST l, const string& time, const string& plate) {
    PELEM vehicle = nullptr;
    for (; l; l = l->next) {
        if (l->plate == plate) {
            vehicle = l;
            break;
        }
    }

    if (!vehicle) {
        return -1;
    }

    int S = 0;
    bool isBike = Check(vehicle) == 1;

    if (isBike) {
        if (vehicle->time <= "06:00:00" && time <= "06:00:00") {
            S = 1;
        }
        else if (vehicle->time >= "18:00:00" && time >= "18:00:00") {
            S = 2;
        }
        else if (vehicle->time <= "06:00:00" && time >= "18:00:00") {
            S = 5;
        }
        else {
            S = 3;
        }
    }
    else {
        if (vehicle->time <= "06:00:00" && time <= "06:00:00") {
            S = 3;
        }
        else if (vehicle->time >= "18:00:00" && time >= "18:00:00") {
            S = 5;
        }
        else if (vehicle->time <= "06:00:00" && time >= "18:00:00") {
            S = 13;
        }
        else {
            S = 8;
        }
    }
    return S;
}

int out(LLIST& l, const string& time, const string& plate) {
    int pos = Find(l, plate);
    if (pos == -1) {
        return 0;
    }
    int cost = Bill(l, time, plate);
    PELEM n = llSeek(l, pos);
    l = llDelete(l, n);
    return cost;
}

LLIST llDeleteAll(LLIST l) {
    PELEM p;
    while (l) {
        p = l->next;
        delete l;
        l = p;
    }
    return NULL;
}

int In(LLIST& l, const string& time, const string& plate) {
    int n = Find(l, plate);
    if (n != -1) {
        return 0;
    }
    l = llInsertTail(l, "", time, plate);
    return 1;
}

int cnt_moto(LLIST l) {
    int count = 0;
    while (l) {
        if (Check(l) == 0) {
            count++;
        }
        l = l->next;
    }
    return count;
}

int main() {
    LLIST L1 = llInit();
    LLIST L2 = llInit();

    while (true) {
        string a, b;
        cin >> a;
        if (a == "*") {
            break;
        }
        cin >> b;
        L1 = llInsertTail(L1, "", a, b);
    }

    int totalBill = 0;

    while (true) {
        string a;
        cin >> a;
        if (a == "***") {
            break;
        }

        if (a == "find") {
            string plate;
            cin >> plate;
            L2 = llInsertTail(L2, a, "", plate);
        }
        else if (a == "in" || a == "out" || a == "bill") {
            string c, b;
            cin >> c >> b;
            L2 = llInsertTail(L2, a, c, b);
        }
        else {
            L2 = llInsertTail(L2, a);
        }
    }

    LLIST command = L2;
    while (command) {
        if (command->data == "list") {
            Print(L1);
        }
        else if (command->data == "find") {
            cout << Find(L1, command->plate) << endl;
        }
        else if (command->data == "in") {
            cout << In(L1, command->time, command->plate) << endl;
        }
        else if (command->data == "out") {
            int bill = out(L1, command->time, command->plate);
            if (bill != 0) {
                totalBill += bill;
                cout << 1 << endl;
            }
            else {
                cout << 0 << endl;
            }
        }
        else if (command->data == "cnt-moto") {
            cout << cnt_moto(L1) << endl;
        }
        else if (command->data == "bill") {
            cout << Bill(L1, command->time, command->plate) << endl;
        }
        else if (command->data == "billall") {
            cout << totalBill << endl;
        }
        command = command->next;
    }

    L1 = llDeleteAll(L1);
    L2 = llDeleteAll(L2);

    return 0;
}
