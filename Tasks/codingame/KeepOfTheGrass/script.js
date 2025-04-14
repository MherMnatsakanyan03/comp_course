/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

let inputs = readline().split(' ');
let width = parseInt(inputs[0]);
let height = parseInt(inputs[1]);
let middle = new coords(Math.floor(width / 2), Math.floor(height / 2));

function coords(x, y) {
    this.x = x;
    this.y = y;
}

// Helper function to calculate Manhattan distance
function heuristic(a, b) {
    return Math.abs(a.x - b.x) + Math.abs(a.y - b.y);
}

// Helper function to get random element from an array
function getRandomElement(arr) {
    return arr[Math.floor(Math.random() * arr.length)];
}

let initMove = true;
let recyclerCount = 0;

// game loop
while (true) {
    let inputs = readline().split(' ');
    let myMatter = parseInt(inputs[0]);
    let oppMatter = parseInt(inputs[1]);
    let coordsOfEnemy = [];
    let myUnits = [];
    let availableSpawns = [];
    let availableBuilds = [];

    let field = [];

    for (let i = 0; i < height; i++) {
        let line = [];

        for (let j = 0; j < width; j++) {
            let inputs = readline().split(' ');
            let cell = {
                x: j,
                y: i,
                scrapAmount: parseInt(inputs[0]),
                owner: parseInt(inputs[1]), // 1 = me, 0 = foe, -1 = neutral
                units: parseInt(inputs[2]),
                recycler: parseInt(inputs[3]),
                canBuild: parseInt(inputs[4]),
                canSpawn: parseInt(inputs[5]),
                inRangeOfRecycler: parseInt(inputs[6])
            };

            if (cell.owner === 1 && cell.units > 0) {
                myUnits.push(new coords(j, i));
            }
            if (cell.owner === 0 && cell.units > 0) {
                coordsOfEnemy.push(new coords(j, i));
            }
            if (cell.owner === 1 && cell.canSpawn === 1) {
                availableSpawns.push(new coords(j, i));
            }
            if (cell.owner === 1 && cell.canBuild === 1) {
                availableBuilds.push(new coords(j, i));
            }

            line.push(cell);
        }
        field.push(line);
    }

    let actions = [];

    // MOVE units
    myUnits.forEach(unit => {
        let nearestUnowned = null;
        let minDistance = Infinity;

        for (let i = 0; i < height; i++) {
            for (let j = 0; j < width; j++) {
                if (field[i][j].owner !== 1 && field[i][j].scrapAmount > 0 && field[i][j].recycler === 0) {
                    let distance = heuristic(unit, new coords(j, i)) + heuristic(new coords(j, i), middle);
                    if (distance < minDistance) {
                        minDistance = distance;
                        nearestUnowned = new coords(j, i);
                    }
                }
            }
        }

        if (nearestUnowned) {
            let dx = nearestUnowned.x - unit.x;
            let dy = nearestUnowned.y - unit.y;
            if (Math.abs(dx) > Math.abs(dy)) {
                actions.push(`MOVE 1 ${unit.x} ${unit.y} ${unit.x + Math.sign(dx)} ${unit.y}`);
            } else {
                actions.push(`MOVE 1 ${unit.x} ${unit.y} ${unit.x} ${unit.y + Math.sign(dy)}`);
            }
        }
    });

    // BUILD Recyclers if possible and no more than 5
    if (recyclerCount < 5) {
        for (let i = 0; i < availableBuilds.length; i++) {
            let cell = availableBuilds[i];
            if (field[cell.y][cell.x].scrapAmount > 1 && field[cell.y][cell.x].inRangeOfRecycler === 0) {
                actions.push(`BUILD ${cell.x} ${cell.y}`);
                recyclerCount++;
                break;
            }
        }
    }

    // SPAWN units at different locations
    if (myMatter >= 10) {
        let borderSpawns = [];
        availableSpawns.forEach(spawn => {
            let adjacentUnowned = false;
            let neighbors = [
                new coords(spawn.x + 1, spawn.y),
                new coords(spawn.x - 1, spawn.y),
                new coords(spawn.x, spawn.y + 1),
                new coords(spawn.x, spawn.y - 1)
            ];

            neighbors.forEach(neighbor => {
                if (neighbor.x >= 0 && neighbor.x < width && neighbor.y >= 0 && neighbor.y < height) {
                    if (field[neighbor.y][neighbor.x].owner !== 1) {
                        adjacentUnowned = true;
                    }
                }
            });

            if (adjacentUnowned) {
                borderSpawns.push(spawn);
            }
        });

        let selectedSpawns = [];
        let remainingMatter = myMatter;

        while (remainingMatter >= 10 && borderSpawns.length > 0) {
            let spawn = getRandomElement(borderSpawns);
            if (field[spawn.y][spawn.x].units === 0) {
                selectedSpawns.push(spawn);
                remainingMatter -= 10;
            }
            borderSpawns = borderSpawns.filter(s => s.x !== spawn.x || s.y !== spawn.y);
        }

        selectedSpawns.forEach(spawn => {
            actions.push(`SPAWN 1 ${spawn.x} ${spawn.y}`);
        });
    }

    if (actions.length > 0) {
        console.log(actions.join('; '));
    } else {
        console.log('WAIT');
    }
}

// console.log('MOVE amount fromX fromY toX toY');
// console.log('BUILD x y');
// console.log('SPAWN amount x y');
// console.log('WAIT');
// console.log('MESSAGE text');