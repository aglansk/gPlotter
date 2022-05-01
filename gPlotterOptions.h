#pragma once
#include "pbPlots.hpp"
#include "supportLib.hpp"
#include <vector>

using namespace std;

class GridLabelsBasic {
protected:
    vector<wchar_t>* CreateStringDecimalFromNumber(double decimal) {
        StringReference* stringReference;

        stringReference = new StringReference();

        /* This will succeed because base = 10. */
        CreateStringFromNumberWithCheck(decimal, 10.0, stringReference);

        return stringReference->string;
    }
    bool CreateStringFromNumberWithCheck(double decimal, double base, StringReference* stringReference) {
        vector<wchar_t>* string;
        double maximumDigits;
        double digitPosition;
        bool hasPrintedPoint, isPositive;
        double i, d;
        bool success;
        CharacterReference* characterReference;
        wchar_t c;

        isPositive = true;
        if (decimal < 0.0) {
            isPositive = false;
            decimal = -decimal;
        }



        if (decimal == 0.0) {
            stringReference->string = toVector(L"0");
            success = true;
        }
        else {
            characterReference = new CharacterReference();

            if (IsInteger(base)) {
                success = true;

                string = new vector<wchar_t>(0.0);

                maximumDigits = GetMaximumDigitsForBase(base);

                digitPosition = GetFirstDigitPosition(decimal, base);

                decimal = round(decimal * pow(base, maximumDigits - digitPosition - 1.0));

                hasPrintedPoint = false;

                if (!isPositive) {
                    string = AppendCharacter(string, '-');
                }

                /* Print leading zeros. */
                if (digitPosition < 0.0) {
                    string = AppendCharacter(string, '0');
                    string = AppendCharacter(string, '.');
                    hasPrintedPoint = true;
                    for (i = 0.0; i < -digitPosition - 1.0; i = i + 1.0) {
                        string = AppendCharacter(string, '0');
                    }
                }

                /* Print number. */
                for (i = 0.0; i < maximumDigits && success; i = i + 1.0) {
                    d = floor(decimal / pow(base, maximumDigits - i - 1.0));

                    if (d >= base) {
                        d = base - 1.0;
                    }

                    if (!hasPrintedPoint && digitPosition - i + 1.0 == 0.0) {
                        if (decimal != 0.0) {
                            string = AppendCharacter(string, '.');
                        }
                        hasPrintedPoint = true;
                    }

                    if (decimal == 0.0 && hasPrintedPoint) {
                    }
                    else {
                        success = GetSingleDigitCharacterFromNumberWithCheck(d, base, characterReference);
                        if (success) {
                            c = characterReference->characterValue;
                            string = AppendCharacter(string, c);
                        }
                    }

                    if (success) {
                        decimal = decimal - d * pow(base, maximumDigits - i - 1.0);
                    }
                }

                if (success) {
                    /* Print trailing zeros. */
                    for (i = 0.0; i < digitPosition - maximumDigits + 1.0; i = i + 1.0) {
                        string = AppendCharacter(string, '0');
                    }

                    stringReference->string = string;
                }
            }
            else {
                success = false;
            }
        }

        /* Done */
        return success;
    }

public:
    GridLabelsBasic() { } // Inittializes class
    ~GridLabelsBasic() { }
    vector<double>* ComputeGridLinePositions(double cMin, double cMax, StringArrayReference* labels, NumberArrayReference* priorities) {
        vector<double>* positions;
        double cLength, p, pMin, pMax, pInterval, pNum, i, num, rem, priority, mode;

        cLength = cMax - cMin;

        p = floor(log10(cLength));
        pInterval = pow(10.0, p);
        /* gives 10-1 lines for 100-10 diff */
        pMin = ceil(cMin / pInterval) * pInterval;
        pMax = floor(cMax / pInterval) * pInterval;
        pNum = Round((pMax - pMin) / pInterval + 1.0);

        mode = 1.0;

        if (pNum <= 3.0) {
            p = floor(log10(cLength) - 1.0);
            /* gives 100-10 lines for 100-10 diff */
            pInterval = pow(10.0, p);
            pMin = ceil(cMin / pInterval) * pInterval;
            pMax = floor(cMax / pInterval) * pInterval;
            pNum = Round((pMax - pMin) / pInterval + 1.0);

            mode = 4.0;
        }
        else if (pNum <= 6.0) {
            p = floor(log10(cLength));
            pInterval = pow(10.0, p) / 4.0;
            /* gives 40-5 lines for 100-10 diff */
            pMin = ceil(cMin / pInterval) * pInterval;
            pMax = floor(cMax / pInterval) * pInterval;
            pNum = Round((pMax - pMin) / pInterval + 1.0);

            mode = 3.0;
        }
        else if (pNum <= 10.0) {
            p = floor(log10(cLength));
            pInterval = pow(10.0, p) / 2.0;
            /* gives 20-3 lines for 100-10 diff */
            pMin = ceil(cMin / pInterval) * pInterval;
            pMax = floor(cMax / pInterval) * pInterval;
            pNum = Round((pMax - pMin) / pInterval + 1.0);

            mode = 2.0;
        }

        positions = new vector<double>(pNum);
        labels->stringArray = new vector<StringReference*>(pNum);
        priorities->numberArray = new vector<double>(pNum);

        for (i = 0.0; i < pNum; i = i + 1.0) {
            num = pMin + pInterval * i;
            positions->at(i) = num;

            /* Always print priority 1 labels. Only draw priority 2 if they can all be drawn. Then, only draw priority 3 if they can all be drawn. */
            priority = 1.0;

            /* Prioritize x.25, x.5 and x.75 lower. */
            if (mode == 2.0 || mode == 3.0) {
                rem = fmod(abs(round(num / pow(10.0, p - 2.0))), 100.0);

                priority = 1.0;
                if (rem == 50.0) {
                    priority = 2.0;
                }
                else if (rem == 25.0 || rem == 75.0) {
                    priority = 3.0;
                }
            }

            /* Prioritize x.1-x.4 and x.6-x.9 lower */
            if (mode == 4.0) {
                rem = fmod(abs(Round(num / pow(10.0, p))), 10.0);

                priority = 1.0;
                if (rem == 1.0 || rem == 2.0 || rem == 3.0 || rem == 4.0 || rem == 6.0 || rem == 7.0 || rem == 8.0 || rem == 9.0) {
                    priority = 2.0;
                }
            }

            /* 0 has lowest priority. */
            if (EpsilonCompare(num, 0.0, pow(10.0, p - 5.0))) {
                priority = 3.0;
            }

            priorities->numberArray->at(i) = priority;

            /* The label itself. */
            labels->stringArray->at(i) = new StringReference();
            if (p < 0.0) {
                if (mode == 2.0 || mode == 3.0) {
                    num = RoundToDigits(num, -(p - 1.0));
                }
                else {
                    num = RoundToDigits(num, -p);
                }
            }
            labels->stringArray->at(i)->string = CreateStringDecimalFromNumber(num);
        }

        return positions;
    }
};

class GridLabelsHour {
protected:
    vector<wchar_t>* CreateStringDecimalFromNumber(double decimal) {
        StringReference* stringReference;

        stringReference = new StringReference();

        /* This will succeed because base = 10. */
        CreateStringFromNumberWithCheck(decimal, 10.0, stringReference);

        return stringReference->string;
    }
    bool CreateStringFromNumberWithCheck(double decimal, double base, StringReference* stringReference) {
        vector<wchar_t>* string;
        bool hasPrintedPoint, isPositive;
        double i, d;
        bool success;
        CharacterReference* characterReference;
        wchar_t c;

        isPositive = true;
        if (decimal < 0.0) {
            isPositive = false;
            decimal = -decimal;
        }

        if (decimal == 0.0) {
            stringReference->string = toVector(L"0:00");
            success = true;
            cout << "0:00" << endl;
        }

        else
        {
            characterReference = new CharacterReference();

            if (IsInteger(base)) {
                success = true;

                string = new vector<wchar_t>(0.0);

                decimal = fmod(decimal, 24.0);

                if (!isPositive) {
                    string = AppendCharacter(string, '-');
                }

                if (decimal < 1.0)
                {
                    string = AppendCharacter(string, '0');
                }
                else if (decimal < 10.0)
                {
                    d = floor(decimal);
                    success = GetSingleDigitCharacterFromNumberWithCheck(d, base, characterReference);
                    if (success) {
                        c = characterReference->characterValue;
                        string = AppendCharacter(string, c);
                    }
                }
                else
                {
                    d = floor(decimal / 10);
                    success = GetSingleDigitCharacterFromNumberWithCheck(d, base, characterReference);
                    if (success) {
                        c = characterReference->characterValue;
                        string = AppendCharacter(string, c);
                    }
                    d = floor(decimal - d * 10);
                    success = GetSingleDigitCharacterFromNumberWithCheck(d, base, characterReference);
                    if (success) {
                        c = characterReference->characterValue;
                        string = AppendCharacter(string, c);
                    }
                }

                string = AppendCharacter(string, ':');
                decimal = (decimal - floor(decimal)) * 60;

                if (floor(decimal) == 0.0)
                {
                    string = AppendCharacter(string, '0');
                    string = AppendCharacter(string, '0');
                }
                else if (floor(decimal) < 10.0)
                {
                    string = AppendCharacter(string, '0');
                    d = floor(decimal);
                    success = GetSingleDigitCharacterFromNumberWithCheck(d, base, characterReference);
                    if (success) {
                        c = characterReference->characterValue;
                        string = AppendCharacter(string, c);
                    }
                }
                else
                {
                    d = floor(decimal / 10);
                    success = GetSingleDigitCharacterFromNumberWithCheck(d, base, characterReference);
                    if (success) {
                        c = characterReference->characterValue;
                        string = AppendCharacter(string, c);
                    }
                    d = floor(decimal - d * 10);
                    success = GetSingleDigitCharacterFromNumberWithCheck(d, base, characterReference);
                    if (success) {
                        c = characterReference->characterValue;
                        string = AppendCharacter(string, c);
                    }
                }
                if (success)
                {
                    stringReference->string = string;
                }
            }
            else {
                success = false;
            }
        }

        /* Done */
        return success;
    }

public:
    GridLabelsHour() { } // Inittializes class
    ~GridLabelsHour() { }
    vector<double>* ComputeGridLinePositions(double cMin, double cMax, StringArrayReference* labels, NumberArrayReference* priorities) {
        vector<double>* positions;
        double cLength, p, pMin, pMax, pInterval, pNum, i, num, rem, priority, mode;

        cLength = cMax - cMin;

        p = floor(log10(cLength));
        pInterval = pow(10.0, p);
        /* gives 10-1 lines for 100-10 diff */
        pMin = ceil(cMin / pInterval) * pInterval;
        pMax = floor(cMax / pInterval) * pInterval;
        pNum = Round((pMax - pMin) / pInterval + 1.0);

        mode = 1.0;

        if (pNum <= 3.0) {
            p = floor(log10(cLength) - 1.0);
            /* gives 100-10 lines for 100-10 diff */
            pInterval = pow(10.0, p);
            pMin = ceil(cMin / pInterval) * pInterval;
            pMax = floor(cMax / pInterval) * pInterval;
            pNum = Round((pMax - pMin) / pInterval + 1.0);

            mode = 4.0;
        }
        else if (pNum <= 6.0) {
            p = floor(log10(cLength));
            pInterval = pow(10.0, p) / 4.0;
            /* gives 40-5 lines for 100-10 diff */
            pMin = ceil(cMin / pInterval) * pInterval;
            pMax = floor(cMax / pInterval) * pInterval;
            pNum = Round((pMax - pMin) / pInterval + 1.0);

            mode = 3.0;
        }
        else if (pNum <= 10.0) {
            p = floor(log10(cLength));
            pInterval = pow(10.0, p) / 2.0;
            /* gives 20-3 lines for 100-10 diff */
            pMin = ceil(cMin / pInterval) * pInterval;
            pMax = floor(cMax / pInterval) * pInterval;
            pNum = Round((pMax - pMin) / pInterval + 1.0);

            mode = 2.0;
        }

        positions = new vector<double>(pNum);
        labels->stringArray = new vector<StringReference*>(pNum);
        priorities->numberArray = new vector<double>(pNum);

        for (i = 0.0; i < pNum; i = i + 1.0) {
            num = pMin + pInterval * i;
            positions->at(i) = num;

            /* Always print priority 1 labels. Only draw priority 2 if they can all be drawn. Then, only draw priority 3 if they can all be drawn. */
            priority = 1.0;

            /* Prioritize x.25, x.5 and x.75 lower. */
            if (mode == 2.0 || mode == 3.0) {
                rem = fmod(abs(round(num / pow(10.0, p - 2.0))), 100.0);

                priority = 1.0;
                if (rem == 50.0) {
                    priority = 2.0;
                }
                else if (rem == 25.0 || rem == 75.0) {
                    priority = 3.0;
                }
            }

            /* Prioritize x.1-x.4 and x.6-x.9 lower */
            if (mode == 4.0) {
                rem = fmod(abs(Round(num / pow(10.0, p))), 10.0);

                priority = 1.0;
                if (rem == 1.0 || rem == 2.0 || rem == 3.0 || rem == 4.0 || rem == 6.0 || rem == 7.0 || rem == 8.0 || rem == 9.0) {
                    priority = 2.0;
                }
            }

            /* 0 has lowest priority. */
            if (EpsilonCompare(num, 0.0, pow(10.0, p - 5.0))) {
                priority = 3.0;
            }

            priorities->numberArray->at(i) = priority;

            /* The label itself. */
            labels->stringArray->at(i) = new StringReference();
            if (p < 0.0) {
                if (mode == 2.0 || mode == 3.0) {
                    num = RoundToDigits(num, -(p - 1.0));
                }
                else {
                    num = RoundToDigits(num, -p);
                }
            }
            labels->stringArray->at(i)->string = CreateStringDecimalFromNumber(num);
        }

        return positions;
    }
};

class gPlotformatOptions {
protected:

public:

};