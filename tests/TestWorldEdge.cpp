/*  RobotControlCenter
    Copyright (C) 2021-2022 Kuppens Brecht

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "TestWorldEdge.h"
#include "TestFactory.h"
#include "game/WorldEdge.h"
#include <math.h>

REGISTER_TESTCLASS(TestWorldEdge);

void TestWorldEdge::constructor_initalizes_to_empty()
{
    WorldEdge w;
    QVERIFY(w.points().isEmpty());
    QCOMPARE(w.count(), 0);
    QCOMPARE(w.z(), 0.0f);
}

void TestWorldEdge::points_are_sorted_in_rotation_direction_x_to_y()
{
    WorldEdge w;

    w.addPoint(0, 1, 0);
    w.addPoint(1, 0, 0);
    w.addPoint(-1, 0, 0);
    w.addPoint(-1, -1, 0);

    QCOMPARE(w.points().at(0), QPointF(-1, -1));
    QCOMPARE(w.points().at(1), QPointF(1, 0));
    QCOMPARE(w.points().at(2), QPointF(0, 1));
    QCOMPARE(w.points().at(3), QPointF(-1, 0));
}

void TestWorldEdge::test_isDirectionInwards_results()
{
    QFETCH(QPointF, pos);
    QFETCH(double, angle);
    QFETCH(bool, expected);

    WorldEdge w;
    w.addPoint(1, -1, 0);
    w.addPoint(1, 1, 0);
    w.addPoint(2, 0, 0);

    QCOMPARE(w.isDirectionInwards(pos, angle), expected);
    QCOMPARE(w.isDirectionInwards(pos, angle + 2 * M_PI), expected);
}

void TestWorldEdge::test_isDirectionInwards_results_data()
{
    QTest::addColumn<QPointF>("pos");
    QTest::addColumn<double>("angle");
    QTest::addColumn<bool>("expected");

    QTest::newRow("west of world pointing at world") << QPointF(0, 0) << 0.0 << true;
    QTest::newRow("east of world pointing at world") << QPointF(3, 0) << M_PI << true;
    QTest::newRow("north of world pointing at world") << QPointF(1.5, 2) << -M_PI_2 << true;
    QTest::newRow("south of world pointing at world") << QPointF(1.5, -2) << M_PI_2 << true;

    QTest::newRow("west of world pointing away") << QPointF(0, 0) << M_PI << false;
    QTest::newRow("east of world pointing away") << QPointF(3, 0) << 0.0 << false;
    QTest::newRow("north of world pointing away") << QPointF(1.5, 2) << M_PI_2 << false;
    QTest::newRow("south of world pointing away") << QPointF(1.5, -2) << -M_PI_2 << false;

    QTest::newRow("inside pointing east") << QPointF(1.5, 0) << 0.0 << true;
    QTest::newRow("inside pointing north") << QPointF(1.5, 0) << M_PI_2 << true;
    QTest::newRow("inside pointing west") << QPointF(1.5, 0) << M_PI << true;
    QTest::newRow("inside pointing south") << QPointF(1.5, 0) << -M_PI_2 << true;
}
