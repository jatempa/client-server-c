const char studentsFile[] = "students.db";
const char careersFile[] = "careers.db";

typedef struct
{
  int id;
  char firstName[50];
  char lastName[50];
  int career_id;
} student;
