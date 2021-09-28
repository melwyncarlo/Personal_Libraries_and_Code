/* Copyright 2021 Melwyn Francis Carlo */

const MSG_INVALID_CMD  = 'Invalid command!';
const MSG_USER_DEAD    = 'Oh, no! You\'ve been terminated.';
const MSG_EMPTY_AMMO   = 'No ammunition in stock at the moment!';
const MSG_AMMO_WASTED  = 'Oops! You shot at nothing.';
const MSG_BRICK_EXISTS = 'A brick already exists!';
const MSG_BRICK_AHEAD  = 'You cannot overstep bricks!';
const MSG_ENEMY_AHEAD  = 'You cannot overstep enemy vectors!';
const MSG_MAX_BOUNDARY = 'You cannot exceed the boundary limits!';
const MSG_LEVEL        = 'Level      :  ';
const MSG_AMMUNITION   = 'Ammunition :  ';
const MSG_UP           = 'UP :  User at position ';
const MSG_DOWN         = 'DOWN :  User at position ';
const MSG_LEFT         = 'LEFT :  User at position ';
const MSG_RIGHT        = 'RIGHT :  User at position ';
const MSG_NEXT_LEVEL   = 'Next Level! User at position ';
const MSG_BRICK_DOWN   = 'Oops! Brick demolished at position ';
const MSG_ENEMY_DOWN   = 'Yayy! Enemy down at position ';
const MSG_WRONG_ANGLE  = 'Only angles between -359 and +359 are accepted.';

const START_BUTTON_STR_DEFAULT    = 'Let\'s start the game!';
const START_BUTTON_STR_LOADING    = 'Loading . . .';
const START_BUTTON_STR_VALIDATING = 'Validating . . .';

const COLOR_DARK_CONTRAST  = '202020';
const COLOR_LIGHT_CONTRAST = 'F5F5F5';
const COLOR_DARK_GRAY      = '404040';
const COLOR_LIGHT_GRAY     = '606060';
const COLOR_BRICK          = 'BD5858';

const COLOR_ENEMY_VICTORY  = '006400';
const COLOR_ENEMY_DEFEAT   = 'FA8072';

const AMMUNITION_RELOAD_TIME_LAPSE    = 10 * 1E3; /* 10 seconds */
const ONE_SECOND                      =  1 * 1E3; /*  1 second */

const ENEMY_LAUNCH_TIME_LAPSE_INITIAL    =    3 * 1E3; /*   3 seconds */
const ENEMY_LAUNCH_TIME_LAPSE_DERIVATIVE = -0.1 * 1E3; /* 0.1 second  */

const ENEMY_STAY_TIME_MIN =  5; /*  5 seconds */
const ENEMY_STAY_TIME_MAX = 20; /* 20 seconds */

const ENEMY_FLASH_PERIOD  =  2 * 1E3; /*  2 seconds */

const ENEMY_FADE_IN_RATE  = 100; /* 100 milli-seconds */
const SEC_PER_ENEMY_FADE_IN_RATE = 1000 / ENEMY_FADE_IN_RATE;

const MAX_N_AMMUNITION = 50;
const MAX_N_ENEMIES = 10;

const ANGLE_MINIMUM = -359;
const ANGLE_MAXIMUM =  359;

const W = 0.90 * screen.availWidth;
const H = 0.65 * screen.availHeight;

const COLS_MINIMUM = 10;
const ROWS_MINIMUM = 5;

const COLS_MAXIMUM = 50;
const ROWS_MAXIMUM = 20;

const CELL_L_MINIMUM = Math.floor((Math.floor(W / COLS_MAXIMUM) + Math.floor(H / ROWS_MAXIMUM)) / 2);
const CELL_L_MAXIMUM = Math.floor((Math.floor(W / COLS_MINIMUM) + Math.floor(H / ROWS_MINIMUM)) / 2);

var ENEMY_LAUNCH_TIME_LAPSE = ENEMY_LAUNCH_TIME_LAPSE_INITIAL;

var ANGLE_IS_COUNTER_CLOCKWISE = true;

var GAME_HAS_BEGUN  = false;

var AMMUNITION_RELOAD_TIMER;
var ENEMY_LAUNCH_TIMER;
var ENEMY_TRACK_TIMER;

var N_AMMUNITION = 10;
var N_ENEMIES = 0;

var LEVEL = 1;

var CELL_L = 35;

var CELL_COLS_N;
var CELL_ROWS_N;

var CELL_I_POS = 
{
    X : 0, 
    Y : 0 
};

var COMMAND_TERM = 
{
    UP    : 'up', 
    DOWN  : 'down', 
    LEFT  : 'left', 
    RIGHT : 'right', 
    BRICK : 'brick', 
    SHOOT : 'shoot' 
};

var CELL_I_IS_BRICK = [];
var CELL_I_IS_ENEMY = [];
var ENEMY_INFO = [];


function print_message(message) {
    document.getElementById("cli-output").value = message;
}

function get_id_str(r, c) {
    return 'r' + (r).toString() + 'c' + (c).toString();
}

function clear_cell(r, c) {
    var cell = document.getElementById(get_id_str(r, c));
    cell.style.backgroundColor = '#' + COLOR_LIGHT_CONTRAST;
    cell.style.opacity         = '1';
    cell.style.boxShadow       = 'none';
    cell.firstElementChild.style.visibility = 'hidden';
}

function set_cell_i(r, c) {
    var cell = document.getElementById(get_id_str(r, c));
    cell.style.backgroundColor = '#' + COLOR_BRICK;
    cell.style.boxShadow       = '0 0 7px 2px #' + COLOR_DARK_GRAY + ' inset';
}

function move_pos(r, c, message) {
    if (!CELL_I_IS_BRICK[r][c]) {
        if (!CELL_I_IS_ENEMY[r][c]) {
            if (!CELL_I_IS_BRICK[CELL_I_POS.Y][CELL_I_POS.X]) {
                clear_cell(CELL_I_POS.Y, CELL_I_POS.X);
            } else {
                var cell_i = document.getElementById(get_id_str(CELL_I_POS.Y, CELL_I_POS.X));
                cell_i.style.backgroundColor = '#' + COLOR_LIGHT_GRAY;
                cell_i.style.boxShadow       = '0 0 4px 2px #' + COLOR_DARK_CONTRAST + ' inset';
            }
            CELL_I_POS.X = c;
            CELL_I_POS.Y = r;
            set_cell_i(CELL_I_POS.Y, CELL_I_POS.X);
            print_message(message + 'R-' + CELL_I_POS.Y + ' C-' + CELL_I_POS.X);
        } else {
            print_message(MSG_ENEMY_AHEAD);
        }
    } else {
        print_message(MSG_BRICK_AHEAD);
    }
}

function build_brick() {
    if (!CELL_I_IS_BRICK[CELL_I_POS.Y][CELL_I_POS.X]) {
        var cell_i = document.getElementById(get_id_str(CELL_I_POS.Y, CELL_I_POS.X));
        cell_i.style.backgroundColor = '#C0C0C0';
        cell_i.style.boxShadow       = '0 0 4px 2px #' + COLOR_DARK_CONTRAST + ' inset';
        CELL_I_IS_BRICK[CELL_I_POS.Y][CELL_I_POS.X] = true;
    } else {
        print_message(MSG_BRICK_EXISTS);
    }
}

function shoot_enemy_vector(angle) {
    if (N_AMMUNITION == 0) {
        print_message(MSG_EMPTY_AMMO);
        return;
    }

    N_AMMUNITION--;

    document.getElementById("amunition-status").value = MSG_AMMUNITION + N_AMMUNITION.toString();

    if (!ANGLE_IS_COUNTER_CLOCKWISE) {
        angle = (360 - angle) % 360;
    }

    if (angle < 0) {
        angle += 360;
    }

    var slope = Math.tan(angle * Math.PI / 180);

    var cells_on_path = [];

    if (angle == 0) {
        for (var i = (CELL_I_POS.X + 1); i <= CELL_COLS_N; i++) {
            cells_on_path.push([CELL_I_POS.Y, i]);
        }
    } else if (angle == 90) {
        for (var i = (CELL_I_POS.Y + 1); i <= CELL_ROWS_N; i++) {
            cells_on_path.push([i, CELL_I_POS.X]);
        }
    } else if (angle == 180) {
        for (var i = (CELL_I_POS.X - 1); i > 0; i--) {
            cells_on_path.push([CELL_I_POS.Y, i]);
        }
    } else if (angle == 270) {
        for (var i = (CELL_I_POS.Y - 1); i > 0; i--) {
            cells_on_path.push([i, CELL_I_POS.X]);
        }
    } else if (angle > 90 && angle < 270) {
        for (var i = (CELL_I_POS.X - 1); i > 0; i--) {
            var j = ((i - CELL_I_POS.X) * slope) + CELL_I_POS.Y;
            if ((j % 0.5) == 0) {
                j = ((i + 0.1 - CELL_I_POS.X) * slope) + CELL_I_POS.Y;
            }
            if (j > 0 && j <= CELL_ROWS_N) {
                cells_on_path.push([Math.round(j), i]);
            }
            if (cells_on_path.length == 0) {
                if (Math.abs(slope) < 1) {
                    shoot_enemy_vector(180);
                } else {
                    if (slope > 0) {
                        shoot_enemy_vector(270);
                    } else {
                        shoot_enemy_vector(90);
                    }
                }
                return;
            }
        }
    } else { /* if ((angle > 0 && angle < 90) || (angle > 270)) */
        for (var i = (CELL_I_POS.X + 1); i <= CELL_COLS_N; i++) {
            var j = ((i - CELL_I_POS.X) * slope) + CELL_I_POS.Y;
            if ((j % 0.5) == 0) {
                j = ((i - 0.1 - CELL_I_POS.X) * slope) + CELL_I_POS.Y;
            }
            if (j > 0 && j <= CELL_ROWS_N) {
                cells_on_path.push([Math.round(j), i]);
            }
            if (cells_on_path.length == 0) {
                if (Math.abs(slope) < 1) {
                    shoot_enemy_vector(0);
                } else {
                    if (slope > 0) {
                        shoot_enemy_vector(90);
                    } else {
                        shoot_enemy_vector(270);
                    }
                }
                return;
            }
        }
    }

    var brick_or_enemy_is_down = false;

    for (var i = 0; i < cells_on_path.length; i++) {
        if (CELL_I_IS_BRICK[cells_on_path[i][0]][cells_on_path[i][1]]) {

            CELL_I_IS_BRICK[cells_on_path[i][0]][cells_on_path[i][1]] = false;
            print_message(MSG_BRICK_DOWN +  'R-' + cells_on_path[i][0].toString() 
                                         + ' C-' + cells_on_path[i][1].toString());

            var brick_cell_i = document.getElementById(get_id_str(cells_on_path[i][0], cells_on_path[i][1]));

            brick_cell_i.style.backgroundColor = '#' + COLOR_ENEMY_DEFEAT;

            setTimeout(function() {
                clear_cell(cells_on_path[i][0], cells_on_path[i][1]);
            }, ONE_SECOND);

            brick_or_enemy_is_down = true;

            break;

        } else if (CELL_I_IS_ENEMY[cells_on_path[i][0]][cells_on_path[i][1]]) {

            N_ENEMIES--;

            CELL_I_IS_ENEMY[cells_on_path[i][0]][cells_on_path[i][1]] = false;
            print_message(MSG_ENEMY_DOWN +  'R-' + cells_on_path[i][0].toString() 
                                         + ' C-' + cells_on_path[i][1].toString());

            var enemy_cell_i = document.getElementById(get_id_str(cells_on_path[i][0], cells_on_path[i][1]));

            enemy_cell_i.style.backgroundColor = '#' + COLOR_ENEMY_DEFEAT;

            enemy_cell_i.style.opacity = '1';

            for (var j = 0; j < ENEMY_INFO.length; j++) {
                if (ENEMY_INFO[j][0] == cells_on_path[i][0] 
                &&  ENEMY_INFO[j][1] == cells_on_path[i][1]) {
                    ENEMY_INFO.splice(j, 1);
                    break;
                }
            }

            setTimeout(function() {
                clear_cell(cells_on_path[i][0], cells_on_path[i][1]);
            }, ENEMY_FLASH_PERIOD);

            brick_or_enemy_is_down = true;

            break;
        }
    }

    if (!brick_or_enemy_is_down) {
        print_message(MSG_AMMO_WASTED);
    }
}

function launch_ammunition_reload() {
    AMMUNITION_RELOAD_TIMER = setInterval(function() {
        if (N_AMMUNITION != MAX_N_AMMUNITION) {
            N_AMMUNITION++;
            document.getElementById("amunition-status").value = MSG_AMMUNITION + N_AMMUNITION.toString();
        }
    }, AMMUNITION_RELOAD_TIME_LAPSE);
}

function launch_enemies() {
    ENEMY_LAUNCH_TIMER = setInterval(function() {
        if (N_ENEMIES != MAX_N_ENEMIES) {
            N_ENEMIES++;

            var random_cell_r = Math.floor(Math.random() * CELL_ROWS_N) + 1;
            var random_cell_c = Math.floor(Math.random() * CELL_COLS_N) + 1;

            while ((CELL_I_POS.Y == random_cell_r 
            &&      CELL_I_POS.X == random_cell_c) 
            ||      CELL_I_IS_BRICK[random_cell_r][random_cell_c] 
            ||      CELL_I_IS_ENEMY[random_cell_r][random_cell_c]) {
                random_cell_r = Math.floor(Math.random() * CELL_ROWS_N) + 1;
                random_cell_c = Math.floor(Math.random() * CELL_COLS_N) + 1;
            }

            var random_angle = 360 - Math.floor(Math.random() * 360) - 1;

            var random_cell = document.getElementById(get_id_str(random_cell_r, random_cell_c));

            random_cell.style.color           = '#' + COLOR_LIGHT_CONTRAST;
            random_cell.style.backgroundColor = '#' + COLOR_DARK_CONTRAST;
            random_cell.style.opacity         = '0';
            random_cell.style.boxShadow       = 'none';

            random_cell.firstElementChild.style.transform = 'rotate(-' 
                                                          + random_angle.toString() 
                                                          + 'deg)';

            random_cell.firstElementChild.style.visibility = 'visible';

            var random_time = (ENEMY_STAY_TIME_MIN 
                            + Math.floor(Math.random() * (ENEMY_STAY_TIME_MAX - ENEMY_STAY_TIME_MIN + 1)))
                            * SEC_PER_ENEMY_FADE_IN_RATE;

            ENEMY_INFO.push([random_cell_r, random_cell_c, random_angle, random_time, random_time]);

            CELL_I_IS_ENEMY[random_cell_r][random_cell_c] = true;
        }
    }, ENEMY_LAUNCH_TIME_LAPSE);
}

function get_angle(y1, x1, y2, x2) {
    return_angle = Math.atan2(y2 - y1, x2 - x1) * 180 / Math.PI;

    /*if (return_angle < 0) {
        return_angle += 360;
    }*/

    return return_angle;
}

function track_enemies() {
    ENEMY_TRACK_TIMER = setInterval(function() {
        for (var i = 0; i < ENEMY_INFO.length; i++) {
            ENEMY_INFO[i][4]--;
            if (ENEMY_INFO[i][4] != 0) {
                var enemy_cell_i = document.getElementById(get_id_str(ENEMY_INFO[i][0], ENEMY_INFO[i][1]));

                var temp_calc = (ENEMY_INFO[i][3] - ENEMY_INFO[i][4]) / ENEMY_INFO[i][3];
                enemy_cell_i.style.opacity = temp_calc.toString();

            } else {
                cell_i_corner_angles = [];
                cell_i_corner_angles.push(get_angle(ENEMY_INFO[i][0], ENEMY_INFO[i][1], CELL_I_POS.Y + 0.5, CELL_I_POS.X - 0.5));
                cell_i_corner_angles.push(get_angle(ENEMY_INFO[i][0], ENEMY_INFO[i][1], CELL_I_POS.Y + 0.5, CELL_I_POS.X + 0.5));
                cell_i_corner_angles.push(get_angle(ENEMY_INFO[i][0], ENEMY_INFO[i][1], CELL_I_POS.Y - 0.5, CELL_I_POS.X - 0.5));
                cell_i_corner_angles.push(get_angle(ENEMY_INFO[i][0], ENEMY_INFO[i][1], CELL_I_POS.Y - 0.5, CELL_I_POS.X + 0.5));

                var min_angle = Math.min(...cell_i_corner_angles);
                var max_angle = Math.max(...cell_i_corner_angles);

                if (min_angle < 0) {
                    min_angle += 360;
                }
                if (max_angle < 0) {
                    max_angle += 360;
                }   

                if (ENEMY_INFO[i][2] > min_angle 
                &&  ENEMY_INFO[i][2] < max_angle) {

                    print_message(MSG_USER_DEAD);
                    clearInterval(ENEMY_LAUNCH_TIMER);
                    clearInterval(ENEMY_TRACK_TIMER);
                    GAME_HAS_BEGUN = false;

                    var enemy_cell_i = document.getElementById(get_id_str(ENEMY_INFO[i][0], ENEMY_INFO[i][1]));

                    enemy_cell_i.style.backgroundColor = '#' + COLOR_ENEMY_VICTORY;

                    break;

                } else {
                    N_ENEMIES--;
                    CELL_I_IS_ENEMY[ENEMY_INFO[i][0]][ENEMY_INFO[i][1]] = false;
                    clear_cell(ENEMY_INFO[i][0], ENEMY_INFO[i][1]);
                    ENEMY_INFO.splice(i, 1);
                    break;
                }
            }
        }
    }, ENEMY_FADE_IN_RATE);
}

function clear_grid() {
    for (var i = 1; i <= CELL_COLS_N; i++) {
        for (var j = 1; j <= CELL_ROWS_N; j++) {
            if (i == CELL_I_POS.X && j == CELL_I_POS.Y) {
                continue;
            }
            clear_cell(j, i);
            CELL_I_IS_BRICK[j][i] = false;
            CELL_I_IS_ENEMY[j][i] = false;
        }
    }

    N_ENEMIES = 0;

    ENEMY_INFO = [];
}

function increment_level() {
    LEVEL++;
    clearInterval(ENEMY_LAUNCH_TIMER);
    clearInterval(ENEMY_TRACK_TIMER);
    clear_grid();
    ENEMY_LAUNCH_TIME_LAPSE += ENEMY_LAUNCH_TIME_LAPSE_DERIVATIVE;
    track_enemies();
    launch_enemies();
    document.getElementById('level-status').value = MSG_LEVEL + LEVEL.toString();
    move_pos(1, CELL_I_POS.X, MSG_NEXT_LEVEL);
}

function set_grid_cell_size() {

    CELL_L = parseInt(document.getElementById('grid-cell-size').value)

    if (Number.isNaN(CELL_L)) {
        alert('The grid cell size must contain only numeric digits!');
        return false;
    }

    if (CELL_L < CELL_L_MINIMUM) {
        alert('The grid cell size is too small!');
        return false;
    }

    if (CELL_L > CELL_L_MAXIMUM) {
        alert('The grid cell size is too large!');
        return false;
    }

    CELL_COLS_N = Math.floor(W / CELL_L);
    CELL_ROWS_N = Math.floor(H / CELL_L);

    return true;
}

function set_command_term(command_term_str) {
    var input_command_term_str;

    if (command_term_str == COMMAND_TERM.UP) {
         input_command_term_str  = document.getElementById(COMMAND_TERM.UP.toLowerCase()    + '-command').value
    } else if (command_term_str == COMMAND_TERM.DOWN) {
         input_command_term_str  = document.getElementById(COMMAND_TERM.DOWN.toLowerCase()  + '-command').value
    } else if (command_term_str == COMMAND_TERM.LEFT) {
         input_command_term_str  = document.getElementById(COMMAND_TERM.LEFT.toLowerCase()  + '-command').value
    } else if (command_term_str == COMMAND_TERM.RIGHT) {
         input_command_term_str  = document.getElementById(COMMAND_TERM.RIGHT.toLowerCase() + '-command').value
    } else if (command_term_str == COMMAND_TERM.BRICK) {
         input_command_term_str  = document.getElementById(COMMAND_TERM.BRICK.toLowerCase() + '-command').value
    } else { /* if (command_term_str == COMMAND_TERM.SHOOT) */
         input_command_term_str  = document.getElementById(COMMAND_TERM.SHOOT.toLowerCase() + '-command').value
    }

    input_command_term_str.trim();

    if (!/^[a-zA-Z]+$/.test(input_command_term_str)) {
        alert('Only alphabetic characters are permitted in the text boxes!');
        return false;
    }

    if (input_command_term_str.length < 2) {
        alert('A minimum of two characters must be present in each of the text boxes!');
        return false;
    }

    if (command_term_str == COMMAND_TERM.UP) {
        COMMAND_TERM.UP    = input_command_term_str;
    } else if (command_term_str == COMMAND_TERM.DOWN) {
        COMMAND_TERM.DOWN  = input_command_term_str;
    } else if (command_term_str == COMMAND_TERM.LEFT) {
        COMMAND_TERM.LEFT  = input_command_term_str;
    } else if (command_term_str == COMMAND_TERM.RIGHT) {
        COMMAND_TERM.RIGHT = input_command_term_str;
    } else if (command_term_str == COMMAND_TERM.BRICK) {
        COMMAND_TERM.BRICK = input_command_term_str;
    } else { /* if (command_term_str == COMMAND_TERM.SHOOT) */
        COMMAND_TERM.SHOOT = input_command_term_str;
    }

    return true;
}

function start_game() {
    var count = 5;
    var timer = setInterval(function() {
        count--;
        if (count != 0) {
            print_message('The game begins in ' + count.toString() + ' ...');
        } else {
            clearInterval(timer);
            GAME_HAS_BEGUN = true;
            print_message('The game has now begun!');
            launch_ammunition_reload();
            track_enemies();
            launch_enemies();
        }
    }, ONE_SECOND);
}

function load_game() {
    var table = document.getElementById('grid');

    var is_brick_row_elements;
    var is_enemy_row_elements;

    for (var i = 0; i < CELL_ROWS_N; i++) {

        var row = table.insertRow(i);

        is_brick_row_elements = [false];
        is_enemy_row_elements = [false];

        for (var j = 0; j < CELL_COLS_N; j++) {

            is_brick_row_elements.push(false);
            is_enemy_row_elements.push(false);

            var cell_ij = row.insertCell(j);

            cell_ij.id = get_id_str(CELL_ROWS_N - i, j + 1);

            cell_ij.style.width  = CELL_L.toString() + 'px';
            cell_ij.style.height = CELL_L.toString() + 'px';

            cell_ij.style.border          = 'solid 1px black';
            cell_ij.style.textAlign       = 'center';
            cell_ij.style.backgroundColor = '#' + COLOR_LIGHT_CONTRAST;

            var div_elem = document.createElement("DIV");

            div_elem.innerHTML = '⮕';

            div_elem.style.visibility = 'hidden';

            cell_ij.appendChild(div_elem);

        }

        CELL_I_IS_BRICK.push(is_brick_row_elements);
        CELL_I_IS_ENEMY.push(is_enemy_row_elements);
    }

    CELL_I_IS_BRICK.push(is_brick_row_elements);
    CELL_I_IS_ENEMY.push(is_enemy_row_elements);

    CELL_I_POS.X = Math.ceil(CELL_COLS_N / 2);
    CELL_I_POS.Y = 1;

    var cell_i = document.getElementById(get_id_str(CELL_I_POS.Y, CELL_I_POS.X));

    cell_i.style.backgroundColor = '#' + COLOR_BRICK;
    cell_i.style.boxShadow       = '0 0 7px 2px #' + COLOR_DARK_GRAY + ' inset';

    document.getElementById("amunition-status").value = MSG_AMMUNITION + N_AMMUNITION.toString();
}

function validate_options() {
    document.getElementById('button-element').disabled  = true;
    document.getElementById('button-element').innerHTML = START_BUTTON_STR_VALIDATING;

    if (!set_grid_cell_size()) {
        document.getElementById('button-element').disabled  = false;
        document.getElementById('button-element').innerHTML = START_BUTTON_STR_DEFAULT;
        return;
    }

    if (!set_command_term(COMMAND_TERM.UP)) {
        document.getElementById('button-element').innerHTML = START_BUTTON_STR_DEFAULT;
        document.getElementById('button-element').disabled  = false;
        return;
    }

    if (!set_command_term(COMMAND_TERM.DOWN)) {
        document.getElementById('button-element').innerHTML = START_BUTTON_STR_DEFAULT;
        document.getElementById('button-element').disabled  = false;
        return;
    }

    if (!set_command_term(COMMAND_TERM.LEFT)) {
        document.getElementById('button-element').innerHTML = START_BUTTON_STR_DEFAULT;
        document.getElementById('button-element').disabled  = false;
        return;
    }

    if (!set_command_term(COMMAND_TERM.RIGHT)) {
        document.getElementById('button-element').innerHTML = START_BUTTON_STR_DEFAULT;
        document.getElementById('button-element').disabled  = false;
        return;
    }

    if (!set_command_term(COMMAND_TERM.BRICK)) {
        document.getElementById('button-element').innerHTML = START_BUTTON_STR_DEFAULT;
        document.getElementById('button-element').disabled  = false;
        return;
    }

    if (!set_command_term(COMMAND_TERM.SHOOT)) {
        document.getElementById('button-element').innerHTML = START_BUTTON_STR_DEFAULT;
        document.getElementById('button-element').disabled  = false;
        return;
    }

    document.getElementById('button-element').innerHTML = START_BUTTON_STR_LOADING;
    load_game();
    document.getElementById('welcome-container').style.display = 'none';
    document.getElementById('main-container').style.display = 'inline';
    document.getElementById('cli-input').focus();
    start_game();
}

document.addEventListener("keyup", function(event) {
    if (event.keyCode === 13) {
        if (document.activeElement === document.getElementById("cli-input")) {
            event.preventDefault();
            run_command();
        }
    }
});

function run_command(e) {
    if (GAME_HAS_BEGUN) {

        var command_str = document.getElementById("cli-input").value.trim().toLowerCase();
        document.getElementById("cli-input").value = "";

        if (command_str.length == 0) {
            return;
        }

        if (command_str == COMMAND_TERM.UP) {
            if (CELL_I_POS.Y < CELL_ROWS_N) {
                move_pos(CELL_I_POS.Y + 1, CELL_I_POS.X, MSG_UP);
            } else {
                increment_level();
            }
        } else if (command_str == COMMAND_TERM.DOWN) {
            if (CELL_I_POS.Y > 1) {
                move_pos(CELL_I_POS.Y - 1, CELL_I_POS.X, MSG_DOWN);
            } else {
                print_message(MSG_MAX_BOUNDARY);
            }
        } else if (command_str == COMMAND_TERM.LEFT) {
            if (CELL_I_POS.X > 1) {
                move_pos(CELL_I_POS.Y, CELL_I_POS.X - 1, MSG_LEFT);
            } else {
                print_message(MSG_MAX_BOUNDARY);
            }
        } else if (command_str == COMMAND_TERM.RIGHT) {
            if (CELL_I_POS.X < CELL_COLS_N) {
                move_pos(CELL_I_POS.Y, CELL_I_POS.X + 1, MSG_RIGHT);
            } else {
                print_message(MSG_MAX_BOUNDARY);
            }
        } else if (command_str == COMMAND_TERM.BRICK) {
            build_brick();
        } else if (command_str.includes(COMMAND_TERM.SHOOT)) {
            const shoot_command_string_array = command_str.replace('+', '').split(' ');

            if (shoot_command_string_array[0] == COMMAND_TERM.SHOOT 
            && !Number.isNaN(parseInt(shoot_command_string_array[1]))) {

                if (parseInt(shoot_command_string_array[1]) >= ANGLE_MINIMUM 
                &&  parseInt(shoot_command_string_array[1]) <= ANGLE_MAXIMUM) {
                    shoot_enemy_vector(parseInt(shoot_command_string_array[1]));
                } else {
                    print_message(MSG_WRONG_ANGLE);
                }
            } else {
                print_message(MSG_INVALID_CMD);
            }
        } else {
            print_message(MSG_INVALID_CMD);
        }

    }
}

