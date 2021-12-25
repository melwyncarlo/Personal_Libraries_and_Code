#include <iostream>

#include <boost/multiprecision/cpp_dec_float.hpp>

typedef boost::multiprecision::number<boost::multiprecision::cpp_dec_float<50>> Precise_Decimal;

/*
    Solves two conic sections equations simultaneously.

    It is used to find the intersection points of two conic sections.

    An arbitrary conic section (equation) is of the following form:

    Ax^2 + Bxy + Cy^2 + Dx + Ey + F = 0

    where A, B, C, D, and E are arbitrary coefficients, and F is an arbitrary constant.

    - by Melwyn Francis Carlo <carlo.melwyn@outlook.com>

    - Written for LibreCAD software.
*/
RS_VectorSolutions RS_Math::simultaneousQuadraticSolverFull(const std::vector<std::vector<double>>& input_conicSections)
{
    RS_VectorSolutions intersectionPoints;

    if (input_conicSections.size() != 2) return intersectionPoints;

    if ((input_conicSections[0].size() == 3) || (input_conicSections[1].size() == 3))
    {
        return simultaneousQuadraticSolverMixed(input_conicSections);
    }

    if ((input_conicSections[0].size() != 6) || (input_conicSections[1].size() != 6))
    {
        return intersectionPoints;
    }

    std::vector<std::vector<double>> conicSections = input_conicSections;

    for (int i = 0; i < 2; i++)
    {
        unsigned int max_orderOfMagnitudeFactor = 0;

        for (int j = 0; j < 6; j++)
        {
            if (std::fabs(conicSections[i][j]) < RS_TOLERANCE15)
            {
                conicSections[i][j] = 0.0;
            }

            if (conicSections[i][j] < 1.0)
            {
                const unsigned int orderOfMagnitudeFactor = (unsigned int) std::trunc(std::fabs(std::log10(conicSections[i][j]))) + 1;

                if (orderOfMagnitudeFactor > max_orderOfMagnitudeFactor)
                {
                    max_orderOfMagnitudeFactor = orderOfMagnitudeFactor;
                }
            }
        }

        for (int j = 0; j < 6; j++)
        {
            const double orderOfMagnitude = std::pow(10, max_orderOfMagnitudeFactor);

            conicSections[i][j] = std::trunc(conicSections[i][j] * orderOfMagnitude * 1.0E+10) * 1.0E-10;
        }
    }

    const Precise_Decimal a1 = conicSections[0][0];
    const Precise_Decimal b1 = conicSections[0][1];
    const Precise_Decimal c1 = conicSections[0][2];
    const Precise_Decimal d1 = conicSections[0][3];
    const Precise_Decimal e1 = conicSections[0][4];
    const Precise_Decimal f1 = conicSections[0][5];

    const Precise_Decimal a2 = conicSections[1][0];
    const Precise_Decimal b2 = conicSections[1][1];
    const Precise_Decimal c2 = conicSections[1][2];
    const Precise_Decimal d2 = conicSections[1][3];
    const Precise_Decimal e2 = conicSections[1][4];
    const Precise_Decimal f2 = conicSections[1][5];

    if ((a1 == 0.0) || (a2 == 0.0) || (c1 == 0.0) || (c2 == 0.0)) return intersectionPoints;

    const Precise_Decimal a1_sq = a1 * a1;
    const Precise_Decimal b1_sq = b1 * b1;
    const Precise_Decimal c1_sq = c1 * c1;
    const Precise_Decimal d1_sq = d1 * d1;
    const Precise_Decimal e1_sq = e1 * e1;
    const Precise_Decimal f1_sq = f1 * f1;

    const Precise_Decimal a2_sq = a2 * a2;
    const Precise_Decimal b2_sq = b2 * b2;
    const Precise_Decimal c2_sq = c2 * c2;
    const Precise_Decimal d2_sq = d2 * d2;
    const Precise_Decimal e2_sq = e2 * e2;
    const Precise_Decimal f2_sq = f2 * f2;

    const Precise_Decimal c1_cu = c1_sq * c1;

    std::vector<double> x_n_terms(5, 0.0);

    /* The x^4 term. */
    x_n_terms[4] = ((a1_sq * c1 * c2_sq)  + (a2_sq * c1_cu)  - (2.0 * a1 * a2 * c1_sq * c2) 
                 + (a2 * b1_sq * c1 * c2) - (a2 * b1 * b2 * c1_sq)  
                 + (a1 * b2_sq * c1_sq)  - (a1 * b1 * b2 * c1 * c2)).convert_to<double>();

    /* The x^3 term. */
    x_n_terms[3] = ((2.0 * a1 * c1 * c2_sq * d1) + (2.0 * a2 * c1_cu * d2) - (2.0 * a1 * c1_sq * c2 * d2) - (2.0 * a2 * c1_sq * c2 * d1) 
                 + (2.0 * a2 * b1 * c1 * c2 * e1) + (b1_sq * c1 * c2 * d2) - (a2 * b1 * c1_sq * e2) - (a2 * b2 * c1_sq * e1) - (b1 * b2 * c1_sq * d2) 
                 + (2.0 * a1 * b2 * c1_sq * e2) + (b2_sq * c1_sq * d1) - (a1 * b1 * c1 * c2 * e2) - (a1 * b2 * c1 * c2 * e1) - (b1 * b2 * c1 * c2 * d1)).convert_to<double>();

    /* The x^2 term. */
    x_n_terms[2] = ((2.0 * a1 * c1 * c2_sq * f1) + (2.0 * a2 * c1_cu * f2) + (c1_cu * d2_sq)  + (c1 * c2_sq * d1_sq)  - (2.0 * a1 * c1_sq * c2 * f2) - (2.0 * a2 * c1_sq * c2 * f1) - (2.0 * c1_sq * c2 * d1 * d2) 
                 + (a2 * c1 * c2 * e1_sq)  + (2.0 * b1 * c1 * c2 * d2 * e1) + (b1_sq * c1 * c2 * f2) - (a2 * c1_sq * e1 * e2) - (b1 * b2 * c1_sq * f2) - (b1 * c1_sq * d2 * e2) - (b2 * c1_sq * d2 * e1) 
                 + (a1 * c1_sq * e2_sq)  + (b2_sq * c1_sq * f1) + (2.0 * b2 * c1_sq * d1 * e2) - (a1 * c1 * c2 * e1 * e2) - (b1 * b2 * c1 * c2 * f1) - (b1 * c1 * c2 * d1 * e2) - (b2 * c1 * c2 * d1 * e1)).convert_to<double>();

    /* The x term. */
    x_n_terms[1] = ((2.0 * c1_cu * d2 * f2) + (2.0 * c1 * c2_sq * d1 * f1) - (2.0 * c1_sq * c2 * d1 * f2) - (2.0 * c1_sq * c2 * d2 * f1) 
                 + (2.0 * b1 * c1 * c2 * e1 * f2) + (c1 * c2 * d2 * e1_sq)  - (b1 * c1_sq * e2 * f2) - (b2 * c1_sq * e1 * f2) - (c1_sq * d2 * e1 * e2) 
                 + (c1_sq * d1 * e2_sq)  + (2.0 * b2 * c1_sq * e2 * f1) - (b1 * c1 * c2 * e2 * f1) - (b2 * c1 * c2 * e1 * f1) - (c1 * c2 * d1 * e1 * e2)).convert_to<double>();

    /* The constant term. */
    x_n_terms[0] = ((c1_cu * f2_sq)  + (c1 * c2_sq * f1_sq)  - (2.0 * c1_sq * c2 * f1 * f2) 
                 + (c1 * c2 * e1_sq * f2) - (c1_sq * e1 * e2 * f2) 
                 + (c1_sq * e2_sq * f1) - (c1 * c2 * e1 * e2 * f1)).convert_to<double>();

    if (RS_DEBUG->getLevel() >= RS_Debug::D_INFORMATIONAL)
    {
        DEBUG_HEADER
        std::cout << std::endl << std::endl 
                  << " (" << x_n_terms[4] << ")x^4 + " 
                  <<  "(" << x_n_terms[3] << ")x^3 + " 
                  <<  "(" << x_n_terms[2] << ")x^2 + " 
                  <<  "(" << x_n_terms[1] << ")x + " 
                  <<  "(" << x_n_terms[0] << ") = 0" 
                  << std::endl << std::endl;

        const double a = x_n_terms[4];
        const double b = x_n_terms[3];
        const double c = x_n_terms[2];
        const double d = x_n_terms[1];
        const double e = x_n_terms[0];

        const double a_sq = a * a;
        const double b_sq = b * b;
        const double c_sq = c * c;
        const double d_sq = d * d;
        const double e_sq = e * e;

        const double a_cu = a * a * a;
        const double b_cu = b * b * b;
        const double c_cu = c * c * c;
        const double d_cu = d * d * d;
        const double e_cu = e * e * e;

        std::cout << " Discriminant (Delta) = " 
                  << (256.0 * a_cu * e_cu) - (192.0 * a_sq * b * d * e_sq) - (128.0 * a_sq * c_sq * e_sq) 
                   + (144.0 * a_sq * c * d_sq * e) - (27.0 * a_sq * d_cu * d) + (144.0 * a * b_sq * c * e_sq) 
                   - (6.0 * a * b_sq * d_sq * e) - (80.0 * a * b * c_sq * d * e) + (18.0 * a * b * c * d_cu) 
                   + (16.0 * a * c_cu * c * e) - (4.0 * a * c_cu * d_sq) - (27.0 * b_cu * b * e_sq) 
                   + (18.0 * b_cu * c * d * e) - (4.0 * b_cu * d_cu) - (4.0 * b_sq * c_cu * e) + (b_sq * c_sq * d_sq) 
                  << std::endl << std::endl;

        std::cout << " P Factor = " 
                  << (8.0 * a * c) - (3.0 * b_sq) 
                  << std::endl << std::endl;

        std::cout << " D Factor = " 
                  << (64.0 * a_cu * e) - (16.0 * a_sq * c_sq) + (16.0 * a * b_sq * c) - (16.0 * a_sq * b * d) - (3 * b_cu * b) 
                  << std::endl << std::endl;
    }

	std::vector<double> rootsAbscissae = quarticSolverFull(x_n_terms);

    if (RS_DEBUG->getLevel() >= RS_Debug::D_INFORMATIONAL)
    {
        std::cout << " Number of quartic roots = " << rootsAbscissae.size() 
                  << std::endl << std::endl;
    }

    for (double& rootX : rootsAbscissae)
    {
        const Precise_Decimal rootXPrecise = rootX;

        const Precise_Decimal sqrtTerm1 = boost::multiprecision::trunc(((((b1 * rootXPrecise) + e1) * ((b1 * rootXPrecise) + e1)) 
                                        - (4.0 * c1 * ((a1 * rootXPrecise * rootXPrecise) + (d1 * rootXPrecise) + f1))) * 1.0E+4) * 1.0E-4;

        const Precise_Decimal sqrtTerm2 = boost::multiprecision::trunc(((((b2 * rootXPrecise) + e2) * ((b2 * rootXPrecise) + e2)) 
                                        - (4.0 * c2 * ((a2 * rootXPrecise * rootXPrecise) + (d2 * rootXPrecise) + f2))) * 1.0E+4) * 1.0E-4;

        if (RS_DEBUG->getLevel() >= RS_Debug::D_INFORMATIONAL)
        {
            std::cout << " RootX square root terms : " 
                      << sqrtTerm1 << ", " << sqrtTerm2 
                      << std::endl << std::endl;
        }

        if ((sqrtTerm1 < 0.0) || (sqrtTerm2 < 0.0)) continue;

        const Precise_Decimal numeratorTerm1 = (-b1 * rootXPrecise) - e1;
        const Precise_Decimal numeratorTerm2 = (-b2 * rootXPrecise) - e2;

        const Precise_Decimal denominatorTerm1 = 2.0 * c1;
        const Precise_Decimal denominatorTerm2 = 2.0 * c2;

        const RS_Vector conic_1_points[2] = 
        {
            RS_Vector(rootX, ((numeratorTerm1 + boost::multiprecision::sqrt(sqrtTerm1)) / denominatorTerm1).convert_to<double>()), 
            RS_Vector(rootX, ((numeratorTerm1 - boost::multiprecision::sqrt(sqrtTerm1)) / denominatorTerm1).convert_to<double>()) 
        };

        const RS_Vector conic_2_points[2] = 
        {
            RS_Vector(rootX, ((numeratorTerm2 + boost::multiprecision::sqrt(sqrtTerm2)) / denominatorTerm2).convert_to<double>()), 
            RS_Vector(rootX, ((numeratorTerm2 - boost::multiprecision::sqrt(sqrtTerm2)) / denominatorTerm2).convert_to<double>()) 
        };

        if (((fabs(conic_1_points[0].x - conic_2_points[0].x) < 1.0E-4) 
        &&   (fabs(conic_1_points[0].y - conic_2_points[0].y) < 1.0E-4)) 
        ||  ((fabs(conic_1_points[0].x - conic_2_points[1].x) < 1.0E-4) 
        &&   (fabs(conic_1_points[0].y - conic_2_points[1].y) < 1.0E-4)))
        {
            intersectionPoints.push_back(conic_1_points[0]);
        }


        if (((fabs(conic_1_points[1].x - conic_2_points[0].x) < 1.0E-4) 
        &&   (fabs(conic_1_points[1].y - conic_2_points[0].y) < 1.0E-4)) 
        ||  ((fabs(conic_1_points[1].x - conic_2_points[1].x) < 1.0E-4) 
        &&   (fabs(conic_1_points[1].y - conic_2_points[1].y) < 1.0E-4)))
        {
            intersectionPoints.push_back(conic_1_points[1]);
        }

        if (RS_DEBUG->getLevel() >= RS_Debug::D_INFORMATIONAL)
        {
            std::cout << " Available roots : " << std::endl 
                      << " 1.1. " << conic_1_points[0] << std::endl 
                      << " 1.2. " << conic_1_points[1] << std::endl 
                      << " 2.1. " << conic_2_points[0] << std::endl 
                      << " 2.2. " << conic_2_points[1] << std::endl << std::endl;

            std::cout << " Chosen root : " << intersectionPoints [intersectionPoints.size() - 1] 
                      << std::endl << std::endl;
        }
    }

    if (RS_DEBUG->getLevel() >= RS_Debug::D_INFORMATIONAL)
    {
        std::cout << " Number of intersection points = " << intersectionPoints.size() 
                  << std::endl << std::endl;
    }

    return intersectionPoints;
}

