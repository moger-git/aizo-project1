//
// Created by moger on 3/22/26.
//

#include "file_reader.h"

#include <fstream>
#include <iostream>
using namespace std;

void file_reader::file_open()
{
    ifstream file("data.csv");
    if (!file)
    {
        cout << "The file cannot be opened!" << endl;
    }

}


void file_reader::file_reader_tab()
{
    int n;
    file >> n;
    for (int i = 0; i < n; i++) {
        file >> tab[i];
    }
}

void file_reader::file_reader_single_linked_list()
{
    int n;
    file >> n;
    for (int i = 0; i < n; i++)
    {
        int x;
        file >> x;
    }
}

void file_reader::file_reader_double_linked_list()
{
    int n;
    file >> n;
    for (int i = 0; i < n; i++)
    {
        int x;
        file >> x;
    }
}

