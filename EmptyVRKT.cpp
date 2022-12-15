#include <iostream>
#include <iomanip>
#include <fcntl.h>
#include <io.h>
#include <windows.h>
#include <fcntl.h>
using namespace std;

const double k = 13, e = 2.7182818284f; // constants
double T = 840; // время работы двигателя

char ravn = (char)(169 + 36), lu_corner = (char)(169 + 32), ru_corner = (char)(169 + 18), ld_corner = (char)(169 + 31),
     rd_corner = (char)(169 + 19), ls_trait = (char)(169 + 35), rs_trait = (char)(169 + 16), strait = (char)(169 + 17);

const size_t width = 9;
const char FILL = '0';

string print_line(int cnt=60)
{
    string s = "";
    for (int i = 0; i < cnt; i++)
    {
        s += ravn;
    }
    return s;
}
// перевод км/c в м/c
double km_to_m(double speed)
{
    return speed * 1000;
}

// формула циалковского для одной ступени [1]
// I - удельный импульс
// V - скорость
double fuel_mass(double M0, double I, double V)
{
    // подробнее о данных в "математической модели"
    double e_VI = pow(e, (V / I));
    return ((M0 * k * (e_VI - 1)) / (k + 1 - e_VI));
}


// формула циалковского для многоступечатой ракеты [2]
double get_V(double m11, double m12) // m1,m2 масса заправленых ступеней, посчитанная ранее
{
    double i1 = 4600, i2 = 2650, m0 = 5712, m21 = 110460, m22 = 4500;
    return (i1 * log((m0 + m11 + m12) / (m0 + m21 - m11 + m11 + m12)) +
        i2 * log((m0 + m12) / (m0 + m22 - m12 + m12)));
}

// Расчёт окон стартов(синодическим периодом) [3]
double synodic_period(double T1, double T2)
{
    return (T1 * T2) / abs(T2 - T1);
}

// предпосчёт коэфицента k для вычисления массы
double get_koef_k(double fuel) 
{
    return fuel / T;
}

// расчёт сгораемого топлива и v [4]
void mass_change_per_steps(double M, double fuel)
{
    double u = 10000, t = 0.000001, k = get_koef_k(fuel), dM = fuel;
    int steps = 1e3, i = 0;
    double dt = T / steps;
    while (T - dt * i >= 0 && dM >= 0)
    {
        cout << strait << "    " << setw(6) << setfill(FILL) << dt * i << "\t   ";
        cout << setw(width) << setfill(FILL) << dM << "\t    " << setw(width) << setfill(FILL) << fuel - dM << "       " ;
        cout << setw(width) << setfill(FILL) << dM * u / (M - dM) << "    " << strait << "\n";
        dM -= k * dt;
        i++;
    }
}

// красивый вывод сгораемого топлива и V
void beauty_print_mass_per_steps(double fuelMass, double fullMass)
{
    cout << "\t\t\t[Fourth from file]\n";
    cout << "\n \tFuel mass per second and Vector of velociuty\n\n";

    cout << lu_corner << print_line(64) << ru_corner << "\n";
    cout << strait << "    Second        Mass Left       Mass Burned      Vector Vel.  "<< strait << "\n";
    cout << ls_trait << print_line(64) << rs_trait << "\n";
    mass_change_per_steps(fullMass, fuelMass);
    cout << ld_corner << print_line(64) << rd_corner << "\n";
}

void beautu_print_masses_V(double massFuel1step, double massFuel2step, double rocket_mass)
{
    cout << '\t' << "\t\t\t[First and Second from file]\n";
    cout << "\n  Fuel masses for stages got from Tsiolkovsky's and V got from Tsiolkovsky's to several stages\n\n";
    double V = get_V(massFuel1step, massFuel2step);
    cout << '\t' << lu_corner << print_line(75) << ru_corner << "\n";
    cout << '\t' << strait << "    M. rocket         M. first stage       M. second stage      Velosity   " << strait << "\n";
    cout << '\t' << ls_trait << print_line(75) << rs_trait << "\n";
    cout << '\t' << strait << "    " << setw(6) << setfill(FILL) << rocket_mass << "\t\t ";
    cout << setw(width) << setfill(FILL) << massFuel1step << "\t\t";
    cout << setw(width) << setfill(FILL) << massFuel2step << "\t " ;
    cout << setw(width) << setfill(FILL) << V << "  " << strait << "\n";
    cout << '\t' << ld_corner << print_line(75) << rd_corner << "\n\n";
}
void beauty_print_synodic_period()
{
    double T1 = 365; // T Земли
    double T2 = 225; // T Венеры
    double SP = synodic_period(T1, T2);

    cout << "\n\t\t\t[Third from file]\n";
    cout <<  "\n \t Calculation of start points (synodic period)\n\n";
    cout << '\t' << lu_corner << print_line(47) << ru_corner << "\n";
    cout << '\t' << strait << "    Earth T       Venus T     Synodic Period   " << strait << "\n";
    cout << '\t' << ls_trait << print_line(47) << rs_trait << "\n";
    cout << '\t' << strait << "    " << setw(4) << setfill(FILL) << T1 << "\t    ";
    cout << setw(4) << setfill(FILL) << T2 << "\t  ";
    cout << setw(4) << setfill(FILL) << SP << "\t" << strait << "\n";
    cout << '\t' << ld_corner << print_line(47) << rd_corner << "\n\n\n";
}
int main()
{
    SetConsoleTitle(L"EMPTY VRKT");
    SetConsoleCP(437);
    SetConsoleOutputCP(437);

    std::cout << std::fixed;
    std::cout << std::setprecision(2);

    double rocket_mass = 948762; // масса ракеты
    double massFuel1step, massFuel2step; // масса топлива для первой и второй ступеней
    massFuel2step = fuel_mass(2710, 2650, km_to_m(6.6)); // считаем по формуле циалковского первую ступень
    massFuel1step = fuel_mass(3730 + massFuel2step, 4600, km_to_m(4.6)); // считаем по формуле циалковского вторую ступень

    double massFuel = massFuel1step + massFuel2step, full_mass = rocket_mass + massFuel;

    beautu_print_masses_V(massFuel1step, massFuel2step, rocket_mass); //  --> OK
    beauty_print_synodic_period();                                    //  --> OK
    beauty_print_mass_per_steps(massFuel, full_mass);                 //  --> OK
    

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    char ex;
    cout << '\n' << "Type Anything to Exit >>> ";
    cin >> ex;
}