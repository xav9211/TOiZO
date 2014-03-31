#!/usr/bin/env python

import sys
import itertools
import random

class ThisShouldNeverHappenError(Exception): pass

def makePath(origin, points):
    """
    Znajduje sciezke przechodzaca przez wszystkie punkty z kolekcji `points`,
    zaczynajac od `origin`

    Do kazdego punktu z `points` musi istniec sciezka z `origin`, przechodzaca
    tylko przez punkty nalezace do `points`!

    origin - tupla (x, y)
    points - kolekcja tupli (x, y), moze zawierac duplikaty
    """
    if len(points) == 0:
        return [ origin ]

    newOrigins = set() # punkty bezposrednio sasiadujace z `origin`
    newPoints = set() # reszta punktow

    for point in points:
        absDelta = (abs(point[0] - origin[0]),
                    abs(point[1] - origin[1]))

        # max odleglosc na kazdej z osi <= 1
        if ((absDelta[0] <= 1 and absDelta[1] <= 1)
                # oraz punkt nie sasiaduje z `origin` po przekatnej
                and ((absDelta[0] == 1) != (absDelta[1] == 1))):
            newOrigins.add(point)
        elif point != origin:
            newPoints.add(point)

    if len(newOrigins) == 0:
        return [ origin ]

    path = [ origin ]

    # wywolaj rekurencyjnie dla kazdego z bezposrednich sasiadow
    for o in newOrigins:
        nextPath = makePath(o, newPoints)
        # w nastepnych wywolaniach pomin juz odwiedzone punkty
        newPoints -= set(nextPath)
        path += nextPath + [ origin ]

    return path

class Robot(object):
    def __init__(self, x, y):
        self.number = -1
        self.points = set([ (x, y) ])
        self.position = (x, y)
        self.program = None

    @staticmethod
    def pointsToMove(old, new):
        """
        Zwraca kierunek ruchu, jaki trzeba wykonac, zeby przejsc od punktu
        `old` do `new`
        """
        delta = (new[0] - old[0], new[1] - old[1])
        if abs(delta[0]) + abs(delta[1]) != 1:
            raise ThisShouldNeverHappenError()

        if delta[0] == -1: return 'W'
        if delta[0] == 1: return 'E'
        if delta[1] == -1: return 'N'
        if delta[1] == 1: return 'S'

        raise ThisShouldNeverHappenError()

    def generateProgram(self):
        """
        Generuje program (`self.program`) na podstawie punktow, ktore ma
        odwiedzic robot (`self.points`)
        """
        pathPoints = makePath((0, 0), self.points)
        if len(pathPoints) == 1:
            self.program = '0'
            return

        path = [ (Robot.pointsToMove(pathPoints[0], pathPoints[1]), 1) ]

        for a, b in zip(pathPoints[1:], pathPoints[2:]):
            moveDir = Robot.pointsToMove(a, b)
            if path[-1][0] == moveDir:
                path[-1] = (moveDir, path[-1][1] + 1)
            else:
                path.append((moveDir, 1))

        self.program = (str(len(path)) + '\n'
                        + '\n'.join('%s %d' % move for move in path))

def generateRobots(width, height):
    """
    Zwraca liste robotow szczelnie wypelniajacych mape o rozmiarze
    `width` x `height`
    """
    robots = []
    fields = [ 0 ] * (width * height)

    for y in xrange(height):
        for x in xrange(width):
            possibleValues = [ len(robots) ]

            if x > 1: possibleValues.append(fields[y * width + x - 1 ])
            if y > 1: possibleValues.append(fields[(y - 1) * width + x])

            newValue = random.choice(possibleValues)
            fields[y * width + x] = newValue
            if newValue == len(robots):
                robots.append(Robot(x, y))
            else:
                robots[newValue].points.add((x, y))


    return robots

def moveRobotOrigins(robots):
    """
    Ustawia losowy punkt jako ten, w ktorym robot rozpoczyna wykonanie programu
    """
    for r in robots:
        origin = random.sample(r.points, 1)[0]
        r.position = origin

        newPoints = set()
        for point in r.points:
            newPoints.add((point[0] - origin[0],
                           point[1] - origin[1]))

        r.points = newPoints

    return robots

def getNeighbors(point):
    """
    Zwraca liste punktow bezposrednio sasiadujacych z podanym
    """
    return [ (point[0] - 1, point[1]),
             (point[0] + 1, point[1]),
             (point[0], point[1] - 1),
             (point[0], point[1] + 1) ]

def generateFakeRobots(num, minPathPoints, maxPathPoints):
    """
    Zwraca liste robotow bladzacych przypadkowo po sciezce o dlugosci
    z przedzialu [ `minPathPoints`, `maxPathPoints` ]
    """
    robots = []

    for _ in xrange(num):
        r = Robot(0, 0)
        neighbors = getNeighbors((0, 0))

        length = random.randint(minPathPoints, maxPathPoints)
        while len(r.points) < length:
            nextPointIdx = random.randint(0, len(neighbors) - 1)
            nextPoint = neighbors[nextPointIdx]

            neighbors += [ p for p in getNeighbors(nextPoint) if p not in neighbors ]
            r.points.add(nextPoint)

        robots.append(r)

    return robots

def saveInput(inFile, width, height, robots):
    """
    Zapisuje dane wejsciowe (mape i liste robotow) w formacie zgodnym z trescia
    zadania z TOiZO
    """
    with open(inFile, 'w') as f:
        f.write('%d %d\n' % (width, height))
        f.write('%d\n' % (width * height))
        f.write('%d\n' % len(robots))
        for r in robots:
            f.write('%d\n' % r.number)
            f.write('%s\n' % r.program)

def saveOutput(outFile, robots):
    """
    Zapisuje liste robotow jako przykladowe rozwiazanie
    """
    with open(outFile, 'w') as f:
        f.write('%d\n' % len(robots))
        for r in robots:
            f.write('%d %d %d\n' % (r.number, r.position[0] + 1, r.position[1] + 1))

if len(sys.argv) < 5:
    print('usage: %s map_width map_height in_file out_file [ min_fake_robot_path_length max_fake_robot_path_length ]' % sys.argv[0])
    exit(1)

width = int(sys.argv[1])
height = int(sys.argv[2])
inFile = sys.argv[3]
outFile = sys.argv[4]

MIN_PATH_POINTS = 40
MAX_PATH_POINTS = 50

if len(sys.argv) > 5: MIN_PATH_POINTS = int(sys.argv[5])
if len(sys.argv) > 6: MAX_PATH_POINTS = int(sys.argv[6])

print('generating map')
robots = generateRobots(width, height)
print('map split into %d parts' % len(robots))
print('moving origins')
robots = moveRobotOrigins(robots)

print('generating extra robots')
allRobots = robots + generateFakeRobots(width * height * 3 - len(robots),
                                        MIN_PATH_POINTS, MAX_PATH_POINTS)
print('%d total robots' % len(allRobots))

for idx, r in enumerate(allRobots):
    sys.stdout.write('\rgenerating paths... %d/%d' % (idx, len(allRobots)))
    r.generateProgram()
sys.stdout.write('\rgenerating paths... %d/%d\n' % (len(allRobots), len(allRobots)))

print("every day i'm shufflin'")
random.shuffle(allRobots)
for r, num in zip(allRobots, itertools.count(start = 1)):
    r.number = num

print('saving input')
saveInput(inFile, width, height, allRobots)
print('saving output')
saveOutput(outFile, robots)
print('done!')

