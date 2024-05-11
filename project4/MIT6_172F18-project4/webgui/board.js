/**
 * Copyright (c) 2013--2014 MIT License by 6.172 Staff
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 **/

// Board configuration
var STATICS_HOST = '';
var NUM_COLS = 8;
var NUM_ROWS = 8;
//var SPECIAL_BOARDS = {'startpos':'ss2nw6/2nw7/1nw8/nw2nwne5/3SWSE2SE2/6SE3/5SE4/4SE2NN2/10/10 W',
//                       'endgame':'ss7/8/8/8/8/8/8/7NN W'};
//var SPECIAL_BOARDS = {'startpos':'ss3nw5/3nw6/2nw7/1nw8/nw3nwne4/4SWSE3SE/8SE1/7SE2/6SE3/5SE3NN W',
//                      'endgame':'ss9/10/10/10/10/10/10/10/10/9NN W'};
// var SPECIAL_BOARDS = {'startpos':'ss3nw5/3nw2nw3/2nw7/1nw6SE1/nw9/9SE/1nw6SE1/7SE2/3SE2SE3/5SE3NN W',
                      // 'endgame':'ss9/10/10/10/10/10/10/10/10/9NN W'};



//var SPECIAL_BOARDS = {'startpos':'ss3nw3/3nw4/2nw1nw3/1nw3SE1SE/nw1nw3SE1/3SE1SE2/4SE3/3SE3NN W',
//                     'endgame':'ss7/8/8/8/8/8/8/7NN W'};

var SPECIAL_BOARDS = {'startpos':'ss7/3nwse3/2nwse4/1nwse3NW1/1se3NWSE1/4NWSE2/3NWSE3/7NN W',
                      'endgame':'ss7/8/8/8/8/8/8/7NN W'};

// Leiserchess 2014 starting position
//var SPECIAL_BOARDS = {'startpos':'sssesesesesese1/8/8/8/8/8/8/1NWNWNWNWNWNWNN W',
//                       'endgame':'ss7/8/8/8/8/8/8/7NN W'};
//var SPECIAL_BOARDS = {'startpos':'ss7/3nwse3/2nwse4/1nwse3NW1/1se3NWSE1/4NWSE2/3NWSE3/7NN W',
//                      'endgame':'ss7/8/8/8/8/8/8/7NN W'};
//var SPECIAL_BOARDS = {'startpos':'ss7/1sw4NE1/1sw4NE1/1sw4NE1/1sw4NE1/1sw4NE1/1sw4NE1/7NN W',
//                      'endgame':'ss7/8/8/8/8/8/8/7NN W'};
//var SPECIAL_BOARDS = {'startpos':'8/1sw4NE1/1sw4NE1/1sw1nn2NE1/1sw2SS1NE1/1sw4NE1/1sw4NE1/8 W',
//                      'endgame':'ss7/8/8/8/8/8/8/7NN W'};
//var SPECIAL_BOARDS = {'startpos':'8/nw6NE/se6SW/nw2nn3NE/se3SS2SW/nw6NE/se6SW/8 W',
//                      'endgame':'ss7/8/8/8/8/8/8/7NN W'};
//var SPECIAL_BOARDS = {'startpos':'ss7/8/nwnenwnenwnenwne/8/8/SWSESWSESWSESWSE/8/7NN W',
//                      'endgame':'ss7/8/8/8/8/8/8/7NN W'};

// GUI configuration
var PIECE_SIZE = 500 / NUM_COLS;
var PIECE_MARGIN = 1;
var ANIMATE_DURATION = 500;
var BOARD_WIDTH = NUM_COLS * PIECE_SIZE;
var BOARD_HEIGHT = NUM_ROWS * PIECE_SIZE;
var CANVAS_Z_INDEX = 100;

// Constants
var COL_NAMES = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h','i','j'];
var DIRECTIONS = [[-1, -1], [-1, 0], [-1, 1], [0, -1], [0, 1], [1, -1], [1, 0], [1, 1]];
var KING_MAP = {'nn':'u', 'ee':'r', 'ss':'d', 'ww':'l'};
var PAWN_MAP = {'nw':'u', 'ne':'r', 'se':'d', 'sw':'l'};
var PLAYER_MAP = {'B':'black', 'W':'white'};
var KING_INV_MAP = {'u':'nn', 'r':'ee', 'd':'ss', 'l':'ww'};
var PAWN_INV_MAP = {'u':'nw', 'r':'ne', 'd':'se', 'l':'sw'};
var PLAYER_INV_MAP = {'black':'B', 'white':'W'};
var KING_LASER_MAP = {
    'u': {'u':'x', 'r':'x', 'd':'x', 'l':'x'},
    'r': {'u':'x', 'r':'x', 'd':'x', 'l':'x'},
    'd': {'u':'x', 'r':'x', 'd':'x', 'l':'x'},
    'l': {'u':'x', 'r':'x', 'd':'x', 'l':'x'}};
var PAWN_LASER_MAP = {
    'u': {'u':'x', 'r':'u', 'd':'l', 'l':'x'},
    'r': {'u':'x', 'r':'x', 'd':'r', 'l':'u'},
    'd': {'u':'r', 'r':'x', 'd':'x', 'l':'d'},
    'l': {'u':'l', 'r':'d', 'd':'x', 'l':'x'}};

var ZAPPED_PIECE_COLOR = 'rgba(170,220,160,0.4)';
var FROZEN_PIECE_COLOR = 'rgba(200,100,90,0.4)';
var ZOMBIFIED_WHITE_COLOR = 'rgba(191, 0, 255, 0.5)';
var ZOMBIFIED_BLACK_COLOR = 'rgba(255, 153, 0, 0.5)';

var LASER_COLORS = [
    '#ff0000',  // red
    '#008c00',  // green
    '#0000ff',  // blue
    '#8000ff',  // purple
    '#ff8000',  // orange
    '#ffff00',  // yellow
    '#80ff00',  // lime green
    '#00ffff',  // cyan
    '#00bfff',  // light blue
    '#ff00ff',  // pink
    '#000000',  // black
    '#330000',  // brown
    '#808080',  // grey
];

// Piece ID
var piece_count = 0;

var Piece = function(orientation, color, position, board) {
  this.id = 'piece-' + piece_count++;
  this.name = '';
  this.orientation = orientation;
  this.color = color;
  this.position = position; // Ex: a8
  this.pieceImg = null;
  this.board = board;
  this.blocked = false;

  this.fenText = function() {
    var fen = (this.name === 'king') ? KING_INV_MAP[this.orientation] : PAWN_INV_MAP[this.orientation];
    return (this.color === 'white') ? fen.toUpperCase() : fen;
  };

  this.imageSrc = function() {
    if (!this.blocked) {
    return STATICS_HOST + 'images/' + this.name + '-' + this.color + '.png';
    } else {
    return STATICS_HOST + 'images/radio.png';
    }
  };

  // Position relative to board
  var parentPosition = $('#board-parent').position();

  this.left = function() {
    return 2 + parentPosition.left + COL_NAMES.indexOf(this.position[0]) * PIECE_SIZE;
  }

  this.top = function() {
    return 2 + parentPosition.top + (NUM_ROWS - this.position[1] - 1) * PIECE_SIZE;
  }

  this.draw = function() {
    if (this.pieceImg === null) {
      var that = this;
      this.pieceImg = $('<img />');
      this.pieceImg[0].src = this.imageSrc();
      this.pieceImg[0].id = this.id;
      this.pieceImg[0].width = PIECE_SIZE;
      this.pieceImg[0].height = PIECE_SIZE;
      this.pieceImg[0].draggable = true;
      this.pieceImg.on('dragstart', function(e) {
        that.board.setSelectedPiece(that);
      });
      this.pieceImg.click(function(e) {
        that.board.handlePieceClick(that);
      });
      this.pieceImg.addClass('piece');

      this.pieceImg.appendTo('#board-parent');
    }

    this.pieceImg.css({ top: this.top(), left: this.left() });

    var rotate = {'u': 0, 'd': 180, 'l': 270, 'r': 90};
    this.pieceImg.css({rotate: rotate[this.orientation] + 'deg'});
  };

  this.rotate = function(direction, inanimate) {
    // inanimate optional argument, default false
    var orientations = ['u', 'r', 'd', 'l'];
    var idx = orientations.indexOf(this.orientation);
    var rotationString;
    var idxDelta;
    if ((direction === 'R') || (direction === 'r')) {
      idxDelta = 1;
      rotationString = '+=90deg';
    } else if ((direction === 'L') || (direction === 'l')){
      idxDelta = 3;
      rotationString = '-=90deg';
    } else if ((direction === 'U') || (direction === 'u')){
      idxDelta = 2;
      rotationString = '+=180deg';
    } else {
      return;
    }
    idx = (idx + idxDelta) % 4;
    if (!inanimate) {
      this.pieceImg.transition({rotate: rotationString}, ANIMATE_DURATION / 2);
    }
    this.orientation = orientations[idx];
  }

  this.move = function(newPosition, inanimate) {
    // inanimate optional argument, default false
    this.position = newPosition;
    $('.piece').css('z-index', '10');
    $(this.pieceImg).css('z-index', '11');
    if (!inanimate) {
      this.pieceImg.animate({ top: this.top(), left: this.left() }, ANIMATE_DURATION / 2);
    }
  }

  // Laser
  // laserDirection: direction of input laser
  // return: u,d,l,r (output laser), x (dead), s (stop)
  this.laserResult = function(laserDirection) {
    return this.laserMap[this.orientation][laserDirection]
  }

  this.remove = function() {
    this.pieceImg.remove();
  }
}

var King = function(orientation, color, position, board) {
  var that = new Piece(orientation, color, position, board);
  that.name = 'king';
  that.laserMap = KING_LASER_MAP;

  return that;
};

var Pawn = function(orientation, color, position, board) {
  var that = new Piece(orientation, color, position, board);
  that.name = 'pawn';
  that.laserMap = PAWN_LASER_MAP;
  return that;
};

var Move = function(text) {
    var squares = [];
    for (var i = 0; i + 2 <= text.length; i+=2) {
	var sq = text.slice(i, i + 2);
	// check column name
	var okay = false;
	for (var j = 0; j < NUM_COLS; j++) {
	    if (COL_NAMES[j] === sq[0]) {
		okay = true;
		break;
	    }
	}
	if (okay) {
	    // check row name
	    okay = false;
	    for (var j = 0; j < NUM_ROWS; j++) {
		if (j === (1*sq[1])) {
		    okay = true;
		    break;
		}
	    }
	}
	if (okay) {
	    squares.push(sq);
	} else {
	    squares.push(null);
	}
    }

    if (squares.indexOf(null) !== -1) {
	//console.trace();
	this.from = null;
	this.mid = null;
	this.to = null;
    } else if (squares.length === 1) {
	this.from = squares[0];
	this.mid = squares[0];
	this.to = squares[0];
    } else if (squares.length === 2) {
	this.from = squares[0];
	this.mid = squares[0];
	this.to = squares[1];
    } else if (squares.length === 3) {
	this.from = squares[0];
	this.mid = squares[1];
	this.to = squares[2];
    } else {
	//console.trace();
	this.from = null;
	this.mid = null;
	this.to = null;
    }

    if (this.from !== null && text.length % 2 === 1) {
	if (['L','R','U','l','r','u'].indexOf(text[text.length - 1]) !== -1) {
	    this.rot = text[text.length - 1].toUpperCase();
	} else {
	    //console.trace();
	    this.from = null;
	    this.mid = null;
	    this.to = null;
	    this.rot = null;
	}
    } else {
	this.rot = null;
    }

    if (this.from === this.mid && this.mid === this.to && this.rot === null) {
	//console.trace();
	this.from = null;
	this.mid = null;
	this.to = null;
    }

    if (this.from === this.mid && this.from !== this.to && this.rot !== null) {
	this.mid = this.to;
    }

    if (this.from !== this.mid && this.mid !== this.to && this.rot !== null) {
	//console.trace();
	this.from = null;
	this.mid = null;
	this.to = null;
	this.rot = null;
    }

    if (this.from !== this.mid && this.mid !== this.to && this.from === this.to) {
	//console.trace();
	this.from = null;
	this.mid = null;
	this.to = null;
	this.rot = null;
    }

    if (this.from !== null) {
	okay = false;
	
	var col = COL_NAMES.indexOf(this.mid[0])-COL_NAMES.indexOf(this.from[0]);
	var row = (1*this.mid[1])-(1*this.from[1]);
	if (col === 0 && row === 0) {
            okay = true;
	}
	for (var i=0; i<DIRECTIONS.length; i++) {
            if (DIRECTIONS[i][0] === col && DIRECTIONS[i][1] === row) {
		okay = true;
		break;
            }
	}

	if (okay) {
	    okay = false;
	    col = COL_NAMES.indexOf(this.to[0])-COL_NAMES.indexOf(this.mid[0]);
	    row = (1*this.to[1])-(1*this.mid[1]);
	    if (col === 0 && row === 0) {
		okay = true;
	    }
	    for (var i=0; i<DIRECTIONS.length; i++) {
		if (DIRECTIONS[i][0] === col && DIRECTIONS[i][1] === row) {
		    okay = true;
		    break;
		}
	    }
	}

	if (!okay) {
	    console.log('Move: code 7');//console.trace();
	    this.from = null;
	    this.mid = null;
	    this.to = null;
	    this.rot = null;
	}
    }

    this.isFormed = function() {
	return (this.from !== null);
    }

    this.isDoubleMove = function() {
	return (this.from !== this.mid && this.mid !== this.to);
    }

    this.isDoubleRot = function() {
	return (this.from !== this.to && this.rot !== null);
    }

    this.isDouble = function() {
	return (this.isDoubleMove() || this.isDoubleRot());
    }

    this.isMove = function() {
	return (this.from === this.mid && this.mid !== this.to && this.rot === null);
    }

    this.isRot = function() {
	return (this.from === this.to && this.rot !== null);
    }

    this.isBasic = function() {
	return (this.isMove() || this.isRot());
    }

    this.add = function(addMove) {
	//var addMove = new Move(s);
	var ret = new Move('');
	var fail = false;
	
	if (this.from === null || addMove.from === null) {
	    //console.trace();
	    return ret;
	}
	
	if (this.isDoubleMove() || addMove.isDouble()) {
	    //console.trace();
	    return ret;
	}
	
	if (this.isRot() && !addMove.isRot()) {
	    //console.trace();
	    return ret;
	}
	
	if (this.to !== addMove.from) {
	    //console.trace();
	    return ret;
	}
	
	if (this.isMove() && addMove.isMove()) {
	    ret.from = this.from;
	    ret.mid = this.to;
	    ret.to = addMove.to;
	    return ret;
	}
	
	if (this.isMove() && addMove.isRot()) {
	    ret.from = this.from;
	    ret.mid = this.to;
	    ret.to = this.to;
	    ret.rot = addMove.rot;
	    return ret;
	}
	
	if ((this.isRot() || this.isDoubleRot()) && addMove.isRot()) {
	    var DIR_ADD_MAP = {
		'L': {'L':'U', 'R':null, 'U':'R', 'null':'L'},
		'R': {'L':null, 'R':'U', 'U':'L', 'null':'R'},
		'U': {'L':'R', 'R':'L', 'U':null, 'null':'U'},
		'null': {'L':'L', 'R':'R', 'U':'U', 'null':null}};
	    ret.from = this.from;
	    ret.mid = this.to;
	    ret.to = this.to;
	    ret.rot = DIR_ADD_MAP[this.rot][addMove.rot];
	    if (ret.rot === null) {
		if (this.from === addMove.to) {
		    ////console.trace();
		    ret.from = null;
		    ret.mid = null;
		    ret.to = null;
		} else {
		    ret.mid = ret.from;
		}
	    }
	    return ret;
	}
	
	//console.trace();
	return ret;
    }

    this.toString = function() {
	if (this.from === null) {
	    return '';
	}

	var ret = this.from;
	if (this.from !== this.mid) {
	    ret = ret + this.mid;
	}
	if (this.mid !== this.to) {
	    ret = ret + this.to;
	}
	if (this.rot !== null) {
	    ret = ret + this.rot;
	}

	return ret;
    }

    this.revert = function() {
	if (this.from === null) {
	    return '';
	}

	var ret = this.to;
	if (this.mid !== this.to) {
	    ret = ret + this.mid;
	}
	if (this.from !== this.mid) {
	    ret = ret + this.from;
	}
	if (this.rot !== null) {
	    var revertMap = {'L':'R', 'R':'L', 'U':'U', 'l':'r', 'r':'l', 'u':'u', 'null':''};
	    ret = ret + revertMap[this.rot];
	}

	return ret;
    }
}

var Board = function(dontClear) {
  this.board = {};
  for (var i = 0; i < NUM_COLS; i++) {
    this.board[COL_NAMES[i]] = new Array(NUM_ROWS);
  }

  this.startBoardFen = '';
  this.history = [];
  this.historySnapshot = [];
  this.historyActiveLen = 0;
  this.turn = 'white';
  this.potentialMove = '';
  this.potentialRevertMove = '';

  // Timing
  this.DEFAULT_START_TIME = 120;  // 2 min
  this.DEFAULT_INC_TIME = 2;    // 2 sec
  this.blacktime = this.DEFAULT_START_TIME;
  this.whitetime = this.DEFAULT_START_TIME;
  this.blackinc = this.DEFAULT_INC_TIME;
  this.whiteinc = this.DEFAULT_INC_TIME;

  // Selected piece (from the drag-and-drop interface), and also for the
  // click-rotations.
  this.selectedPiece = '';
  this.selectedPieceLeftRotations = 0;

  // Stomped piece2
  this.stompedPiece = null;
  if (this.stompedPiece!=null){
  this.stompedPiece.remove();
  //this.stompedPiece2.remove();
  }
  this.pinned = [];
  this.zombified = [];
  this.setSelectedPiece = function(selected_piece) {
    this.selectedPiece = selected_piece;
  };

    this.mustDouble = false;

    this.handleDrop = function(x, y) {
	console.log('handleDrop at ' + COL_NAMES[x]+[y]);
	if (this.selectedPiece !== null) {
	    $('#move').val(this.selectedPiece.position + COL_NAMES[x] + y);
	    $('#move-button').click();
	}
    }

    this.handlePieceClick = function(selected_piece) {
	console.log('handlePieceClick at ' + selected_piece.position);
	this.setSelectedPiece(selected_piece);
	// TODO: Get rid of this ugly hack.
	$('#move').val(this.selectedPiece.position + 'L');
	$('#move-button').click();
    };

  this.getPiece = function (position) {
    var piece = this.board[position[0]][position[1]];
    if (piece) {
      return piece;
    } else {
      return null;
    }
  }

  // position = [col #, row #]
  this.getPieceCR = function (position) {
    var piece = this.board[COL_NAMES[position[0]]][position[1]];
    if (piece) {
      return piece;
    } else {
      return null;
    }
  }

  // ########## BOARD REP RELATED ##########

  this.trimTurn = function(fen) {
    if (fen) {
      return fen.slice(0,fen.length-2);
    } else {
      return fen;
    }
  }

  // returns starting board status in FEN notation
  // might use special board names instead
  this.getStartBoardFen = function() {
    if (this.startBoardFen in SPECIAL_BOARDS) {
      return this.startBoardFen;
    } else {
      return 'fen ' + this.startBoardFen;
    }
  };

  // returns current board status in FEN notation
  this.getBoardFen = function() {
    var rep = '';
    for (var r = NUM_ROWS-1; r >= 0; r--) {
      var blankCount = 0;
      for (var c = 0; c < NUM_COLS; c++) {
        var piece = this.getPiece(COL_NAMES[c] + r);
        if (piece) {
          if (blankCount !== 0) {
            rep += blankCount;
          }
          rep += piece.fenText();
          blankCount = 0;
        } else {
          blankCount++;
        }
      }
      if (blankCount !== 0) {
        rep += blankCount;
      }
      if (r > 0) {
        rep += '/';
      }
    }
    rep += ' ' + PLAYER_INV_MAP[this.turn];
    return rep;
  };

  // returns starting board status in FEN notation
  // plus move history, only first [step] turns
  this.getBoardFenHistStep = function(step) {
    var rep = this.startBoardFen;
    if (step > this.history.length) {
      step = this.history.length;
    }
    if (step > 0) {
      rep += '#' + this.history.slice(0,step).join('#');
    }
    return rep;
  }

  // returns starting board status in FEN notation
  // plus move history, only first [historyActiveLen] turns
  this.getBoardFenHistCurrent = function() {
    return this.getBoardFenHistStep(this.historyActiveLen);
  }

  // returns starting board status in FEN notation
  // plus move history, all turns
  this.getBoardFenHistFull = function() {
    return this.getBoardFenHistStep(this.history.length);
  }

  this.getHistorySnapshot = function(turn) {
    if (turn >= this.historySnapshot.length) {
      return this.trimTurn(this.getBoardFen());
    } else {
      return this.historySnapshot[turn]
    }
  }

  // takes FEN notation describing ONE row e.g. '10', '4NESE1ss2'
  // returns input separated into tokens of strings and integers
  //     e.g. [4, 'NE', 'SE', 1, 'SS', 2]
  // returns null if input is an invalid string
  this.splitFenRow = function(text) {
    var isDigit = function(char) {
      return '0' <= char && char <= '9';
    };
    var isUppercase = function(char) {
      return 'A' <= char && char <= 'Z';
    };

    var answer = [];
    var colused = 0;
    var i = 0;
    while (i < text.length) {
      if (isDigit(text.charAt(i))) {
        var j = i+1;
        while (j < text.length && isDigit(text.charAt(j))) {
          j++;
        }
        var value = parseInt(text.slice(i,j));
        answer.push(value);
        colused += value;
        i = j;
      } else {
        if (i+1 < text.length && !isDigit(text.charAt(i+1))
            && isUppercase(text.charAt(i)) === isUppercase(text.charAt(i+1))) {
          answer.push(text.slice(i,i+2));
          colused++;
          i += 2;
        } else {
          return null;
        }
      }
    }
    return (colused == NUM_COLS) ? answer : null;
  };

  // takes board status in FEN notation (without move history)
  // sets the board accordingly
  // returns true if board valid, false otherwise
  this.setBoardFen = function(text, dontClear) {
    for (var i = 0; i < NUM_COLS; i++) {
      this.board[COL_NAMES[i]] = new Array(NUM_ROWS);
    }
    var isUppercase = function(char) {
      return 'A' <= char && char <= 'Z';
    };

    text = text.replace(/\s+/g, ''); // Strip spaces
    if (text in SPECIAL_BOARDS) {
      text = SPECIAL_BOARDS[text].replace(/\s+/g, '');
    }
    var turntext = text.slice(text.length-1).toUpperCase();
    if (!(turntext in PLAYER_MAP)) {
        return false;
    }
    var rowtext = text.slice(0,text.length-1).split('/');
    if (rowtext.length === NUM_ROWS) {
      for (var r = NUM_ROWS-1; r >= 0; r--) {
        var cols = this.splitFenRow(rowtext[NUM_ROWS-r-1]);
        if (cols != null) {
          var c = 0;
          var board = this.board;
          var that = this;
          cols.forEach(function(e){
            if (typeof e === 'number') {
              c += e;
            } else {
              var color = isUppercase(e.charAt(0)) ? 'white' : 'black';
              var position = COL_NAMES[c] + r;
              var piece = null;
              if (e.charAt(0) == e.charAt(1)) {
                piece = new King(KING_MAP[e.toLowerCase()], color, position, that);
              } else {
                piece = new Pawn(PAWN_MAP[e.toLowerCase()], color, position, that);
              }
              board[COL_NAMES[c]][r] = piece;
              c++;
            }
          });
        } else {
          return false;
        }
      }
    } else {
      return false;
    }
    this.turn = PLAYER_MAP[turntext];
    if (!dontClear) {
      this.clearLaser();   
    }
    return true;
  };

    // takes board status in FEN notation (with move history)
    // sets the board accordingly
    // returns true if board valid, false otherwise
    this.setBoardFenHist = function(text) {
	console.log('setBoardFenHist: ' + text);
	text = text.replace(/\s+/g, ''); // Strip spaces
	var textarray = text.split('#');
	if (textarray.length > 0) {
	    // Set initial board
	    if (!this.setBoardFen(textarray[0])) {
		return false;
	    }
	    // Set startBoardFen variable
	    if (textarray[0] in SPECIAL_BOARDS) {
		this.startBoardFen = textarray[0];
	    } else {
		this.startBoardFen = textarray[0].slice(0,textarray[0].length-1) + ' ' + textarray[0].slice(textarray[0].length-1);
	    }
	    // Play moves in history
	    this.historyActiveLen = 0;
	    this.historySnapshot = [this.trimTurn(this.getBoardFen())];
	    for (var i = 1; i < textarray.length; i++) {
		if (!this.isKo(textarray[i], this.turn)) {
		    this.move(textarray[i], null, null, true, true);
		    var victim = this.laserTarget(this.turn);
		    if (victim != null) {
			// if (victim) {
			var hitCol = victim.position[0];
			var hitRow = 1*victim.position[1];
			this.board[hitCol][hitRow] = null;
			// }
		    }

		    this.commitPotentialMove();
		    this.turn = ((this.turn === 'black') ? 'white' : 'black');
		} else {
		    return false;
		}
	    }
	    return true;
	} else {
	    return false;
	}
    }

  // takes board status in PGN notation
  // sets the board accordingly
  // returns true if board valid, false otherwise
  this.setBoardPGN = function(text) {
    var textarray = text.split('\n');
    var tokenCount = 0;
    var whiteMove = null;
    var blackMove = null;
    var moves = [];
    var replaceNull = function(str) {
      return (str === 'null') ? '-' : str;
    }
    for (var i = 0; i < textarray.length; i++) {
      var line = textarray[i].trim();
      if (line.length === 0 || line[0] === '[' || line[line.length-1] === ']') {
        continue;
      }
      var linearray = line.split(' ');
      for (var j = 0; j < linearray.length; j++) {
        var token = linearray[j];
        if (token.length === 0) {
          continue;
        }
        tokenCount++;
        switch (tokenCount % 9) {
          case 2: whiteMove = token; break;
          case 6: if (token === '-' || token.indexOf('-') === -1) {
                    if (whiteMove !== 'null' || token !== 'null') {
                      moves.push(replaceNull(whiteMove));
                      moves.push(replaceNull(token));
                    }
                    whiteMove = null;
                  }
                  break;
        }
      }
      if (whiteMove !== null && whiteMove !== 'null') {
        moves.push(replaceNull(whiteMove));
      }
    }
    var boardString = 'startpos#' + moves.join('#');
    return this.setBoardFenHist(boardString);
  }

  // ########## MOVES RELATED ##########

  // switches player and notifies the player to move
    this.switchPlayer = function() {
	console.log('End ' + this.turn);
    if (this.turn === 'black') {
      this.blacktime += this.blackinc;
      this.turn = 'white';
    } else {
      this.turn = 'black';
      this.whitetime += this.whiteinc;
    }
    $('#board-parent').trigger('switchPlayer');
  }

    // returns true if move valid
    // param: text is the move: 'a3R', 'j0j1', etc.
    // param: turn is the color specifying current player's turn: 'white' or 'black'
    this.isValidMove = function(text, turn) {
	//console.log('isValidMove: ' + text);
	var theMove = new Move(text);
	//console.log('isValidMove: ' + theMove.from + '-' + theMove.mid + '-' + theMove.to + '/' + theMove.rot);
	if (theMove.from !== null) {
	    // Starting position contains the player's piece
	    var fromPiece = this.getPiece(theMove.from);
	    var midPiece = this.getPiece(theMove.mid);
	    var toPiece = this.getPiece(theMove.to);
	    
	    if (!fromPiece) {
		return false;
	    }

	    if (fromPiece.color !== turn) {
		return false;
	    }

	    if (theMove.isBasic()) {
		if (theMove.isMove() && toPiece !== null && fromPiece.color === toPiece.color) {
		    return false;
		}
	    }
	    
	    if (theMove.isDouble()) {
		if (!midPiece) {
		    return false;
		}
		
		if (fromPiece.color === midPiece.color) {
		    return false;
		}

		if (theMove.isDoubleMove()) {
		    if (toPiece !== null) {
			return false;
		    }
		}
	    }

	    return true;
	}
	return false;
    }
    
    this.isKo = function(text, turn, lastMove) {
	var theMove = new Move(text);
	if (theMove.rot === null && text !== this.potentialRevertMove) {
	    if (!lastMove) {
		lastMove = this.historyActiveLen;
	    }
	    var lastBoardFen1 = (lastMove >= 0) ? this.getHistorySnapshot(lastMove) : null;
	    // Leiserchess 2014: Only one level of repeat is possible.
	    // var lastBoardFen2 = (lastMove >= 1) ? this.getHistorySnapshot(lastMove-1) : null;
	    var newBoard = new Board();
	    newBoard.setBoardFen(this.getBoardFen());

	    var fromC = theMove.from[0];
	    var fromR = theMove.from[1];
	    var midC = theMove.mid[0];
	    var midR = theMove.mid[1];
	    var toC = theMove.to[0];
	    var toR = theMove.to[1];

	    // Perform swap
	    var temp = newBoard.board[fromC][fromR];
	    newBoard.board[fromC][fromR] = newBoard.board[midC][midR];
	    newBoard.board[midC][midR] = newBoard.board[toC][toR];
	    newBoard.board[toC][toR] = temp;
	    // Leiserchess 2014: Old position is always null
	    // if ((oldC !== newC) || (oldR !== newR)) {
	    //   newBoard.board[oldC][oldR] = null;
	    // }
	    var target = newBoard.laserTarget(turn);
	    if (target !== null) {
		var position = target.position;
		newBoard.board[position[0]][position[1]] = null;
	    }
	    var newBoardFen = this.trimTurn(newBoard.getBoardFen());
	    // return newBoardFen === lastBoardFen1 || newBoardFen === lastBoardFen2;
	    // Leiserchess 2014: Only one level of repeat is possible.
	    return newBoardFen === lastBoardFen1;
	} else {
	    return false;
	}
    }


    this.doMove = function(theMove, inanimate) {
	// inanimate optional argument, default false
	console.log('doMove: ' + theMove.toString());

	var fromPiece = this.getPiece(theMove.from);
	var midPiece = this.getPiece(theMove.mid);
	var toPiece = this.getPiece(theMove.to);
	
	if (!fromPiece) {
	    return;
	}
	
	var fromC = theMove.from[0];
	var fromR = theMove.from[1];
	var midC = theMove.mid[0];
	var midR = theMove.mid[1];
	var toC = theMove.to[0];
	var toR = theMove.to[1];

	// Swap if necessary.
	this.board[fromC][fromR] = midPiece;
	this.board[midC][midR] = toPiece;
	this.board[toC][toR] = fromPiece;

	if (theMove.from === theMove.mid) {
	    fromPiece.move(theMove.to, inanimate);
	    if (toPiece) {
		toPiece.move(theMove.from, inanimate);
	    }
	} else {
	    fromPiece.move(theMove.mid, inanimate);
	    if (midPiece) {
		midPiece.move(theMove.from, inanimate);
	    }
	    if (theMove.mid !== theMove.to) {
		fromPiece.move(theMove.to, inanimate);
		if (toPiece) {
		    toPiece.move(theMove.mid, inanimate);
		}
	    }
	}

	if (theMove.rot !== null) {
	    fromPiece.rotate(theMove.rot, inanimate);
	}
    };
    
    this.move = function(text, successCallback, failureCallback, inanimate, dontCheckValidity) {
	// inanimate optional argument, default false
	console.log('move: ' + text );
	console.log('move: ' + this.potentialMove + '/' + this.potentialRevertMove + '/' + this.mustDouble);
	if ((!dontCheckValidity && !this.isValidMove(text, this.turn)) || this.isKo(text, this.turn)) {
	    if (failureCallback) {
		console.log('move: fail ' + text + ' (invalid or Ko)');
		failureCallback();
	    }
	    return;
	}

	var theMove = new Move(text);
	var currMove = new Move(this.potentialMove);
	
	if (dontCheckValidity && text === this.potentialRevertMove) {
	    console.log('move: looks like a revert');
	    
	    this.mustDouble = false;
	    this.potentialMove = '';
	    this.potentialRevertMove = '';
	} else if (!currMove.isFormed()) {
	    if (theMove.isMove()) {
		var toPiece = this.getPiece(theMove.to);
		if (toPiece !== null) {
		    this.mustDouble = true;
		}
	    }
	    
	    this.potentialMove = text;
	    this.potentialRevertMove = theMove.revert();
	} else if (currMove.isFormed()) {
	    var fullMove = currMove.add(theMove);
	    //console.log('move: fullMove ' + fullMove.toString());
	    if (!fullMove.isFormed() && !(currMove.isRot() && theMove.isRot())) {
		if (failureCallback) {
		    console.log('move: fail ' + text + ' (cannot add moves)');
		    failureCallback();
		}
		return;
	    }

	    if (currMove.isMove() && !this.mustDouble) {
		if (failureCallback) {
		    console.log('move: fail ' + text + ' (cannot extend move)');
		    failureCallback();
		}
		return;
	    }

	    if (fullMove.isDoubleMove()) {
		var toPiece = this.getPiece(theMove.to);
		if (toPiece !== null) {
		    if (failureCallback) {
			console.log('move: fail ' + text + ' (move-move must end in empty square)');
			failureCallback();
		    }
		    return;
		}
	    }

	    this.mustDouble = (!currMove.isRot() && !fullMove.isDouble() && fullMove.isFormed());
	    
	    this.potentialMove = fullMove.toString();
	    this.potentialRevertMove = fullMove.revert();
	}
	
	if (!inanimate) {
	    this.clearLaser();
	}

	this.doMove(theMove, inanimate);
	
	console.log('move: success ' + text);
	console.log('move: ' + this.potentialMove + '/' + this.potentialRevertMove + '/' + this.mustDouble);
	
	if (successCallback) {
	    setTimeout(function(){successCallback();}, ANIMATE_DURATION);
	}
    };

    this.clearPotentialMove = function() {
	this.potentialMove = '';
	this.potentialRevertMove = '';
	this.selectedPiece = null;
	this.selectedPieceLeftRotations = 0;
    };

    this.revertPotentialMove = function() {
	this.move(this.potentialRevertMove, null, null, false, true);
	this.clearPotentialMove();
    };

  this.commitPotentialMove = function() {
    this.history.push(this.potentialMove);
    this.historySnapshot.push(this.trimTurn(this.getBoardFen()));
    this.historyActiveLen++;
    this.clearPotentialMove();
  };

  this.findKing = function(color) {
    for (var r = NUM_ROWS-1; r >= 0; r--) {
      for (var c = 0; c < NUM_COLS; c++) {
        var piece = this.getPiece(COL_NAMES[c] + r);
        if (piece && (piece.color === color) && (piece.name === 'king')) {
          return [c, r];
        }
      }
    }
    return null;
  };

    this.lookAhead = function(lastMove, unsafeCallback) {
	var myColor = this.turn;
	var theirColor = (myColor === 'white') ? 'black' : 'white';

	// my own laser
	var casualty = this.laserTarget(myColor);
	if (casualty) {
	    if ((casualty.color === myColor) && (casualty.name === 'king')) {
		unsafeCallback();
		return;
	    }
	}
	// opponent's laser after they move
	var newBoard = new Board();
	newBoard.setBoardFen(this.getBoardFen());
	if (casualty) {
	    newBoard.board[casualty.position[0]][casualty.position[1]] = null;
	}
	for (var r = NUM_ROWS-1; r>=0; r--) {
	    for (var c = 0; c < NUM_COLS; c++) {
		var piece = newBoard.getPieceCR([c,r]);
		if (piece && (piece.color === theirColor)) {
		    // move
		    for (var i = 0; i < DIRECTIONS.length; i++) {
			var midC = c+DIRECTIONS[i][0];
			var midR = r+DIRECTIONS[i][1];
			
			if (midC < 0 || midC >= NUM_COLS || midR < 0 || midR >= NUM_ROWS) {
			    continue; // out of bound
			}
			
			var midPiece = newBoard.getPieceCR([midC, midR]);

			if (midPiece === null) {
			    var moveString = COL_NAMES[c]+r+COL_NAMES[midC]+midR;
			    if (!this.isKo(moveString, this.turn, this.historyActiveLen+1)) { // checks Ko
				var temp = newBoard.board[COL_NAMES[c]][r];
				newBoard.board[COL_NAMES[c]][r] = newBoard.board[COL_NAMES[midC]][midR];
				newBoard.board[COL_NAMES[midC]][midR] = temp;
				var casualty = newBoard.laserTarget(theirColor);
				if (casualty && (casualty.color === myColor) && (casualty.name === 'king')) {
				    unsafeCallback();
				    return;
				}
				var temp = newBoard.board[COL_NAMES[c]][r];
				newBoard.board[COL_NAMES[c]][r] = newBoard.board[COL_NAMES[midC]][midR];
				newBoard.board[COL_NAMES[midC]][midR] = temp;
			    }
			} else if (midPiece.color === myColor) {
			    // doublemove
			    for (var i2 = 0; i2 < DIRECTIONS.length; i2++) {
				var toC = c+DIRECTIONS[i2][0];
				var toR = r+DIRECTIONS[i2][1];
				
				if (toC < 0 || toC >= NUM_COLS || toR < 0 || toR >= NUM_ROWS) {
				    continue; // out of bound
				}
				
				var toPiece = newBoard.getPieceCR([toC, toR]);

				if (toPiece === null) {
				    var moveString = COL_NAMES[c]+r+COL_NAMES[toC]+toR;
				    if (!this.isKo(moveString, this.turn, this.historyActiveLen+1)) { // checks Ko
					var temp = newBoard.board[COL_NAMES[c]][r];
					newBoard.board[COL_NAMES[c]][r] = newBoard.board[COL_NAMES[midC]][midR];
					newBoard.board[COL_NAMES[midC]][midR] = newBoard.board[COL_NAMES[toC]][toR];
					newBoard.board[COL_NAMES[toC]][toR] = temp;
					var casualty = newBoard.laserTarget(theirColor);
					if (casualty && (casualty.color === myColor) && (casualty.name === 'king')) {
					    unsafeCallback();
					    return;
					}
					var temp = newBoard.board[COL_NAMES[toC]][toR];
					newBoard.board[COL_NAMES[toC]][toR] = newBoard.board[COL_NAMES[midC]][midR];
					newBoard.board[COL_NAMES[midC]][midR] = newBoard.board[COL_NAMES[c]][r];
					newBoard.board[COL_NAMES[c]][r] = temp;
				    }
				}
			    }
			    // rotate
			    var orientations = ['u', 'r', 'd', 'l'];
			    var idx = orientations.indexOf(piece.orientation);
			    var temp = newBoard.board[COL_NAMES[c]][r];
			    newBoard.board[COL_NAMES[c]][r] = newBoard.board[COL_NAMES[midC]][midR];
			    newBoard.board[COL_NAMES[midC]][midR] = temp;
			    for (var i2 = 3; i2 >= 0; i2--) {
				piece.orientation = orientations[(idx+i2)%4];
				var newCasualty = newBoard.laserTarget(theirColor);
				if (newCasualty && (newCasualty.color === myColor) && (newCasualty.name === 'king')) {
				    unsafeCallback();
				    return;
				}
			    }
			    var temp = newBoard.board[COL_NAMES[c]][r];
			    newBoard.board[COL_NAMES[c]][r] = newBoard.board[COL_NAMES[midC]][midR];
			    newBoard.board[COL_NAMES[midC]][midR] = temp;
			}
		    }
		    // rotate
		    var orientations = ['u', 'r', 'd', 'l'];
		    var idx = orientations.indexOf(piece.orientation);
		    for (var i = 3; i >= 0; i--) {
			piece.orientation = orientations[(idx+i)%4];
			var newCasualty = newBoard.laserTarget(theirColor);
			if (newCasualty && (newCasualty.color === myColor) && (newCasualty.name === 'king')) {
			    unsafeCallback();
			    return;
			}
		    }
		}
	    }
	}
    };

    this.checkBoard = function() {
	var err = false;
	
	for (var r = NUM_ROWS-1; r>=0; r--) {
	    for (var c = 0; c < NUM_COLS; c++) {
		var piece = this.getPieceCR([c,r]);
		if (piece && piece.position !== COL_NAMES[c]+r) {
		    err = true;
		    console.log(piece.position + '!=='+ COL_NAMES[c]+r);
		}
	    }
	}
	if (err) {
	    alert('bad board');
	}
    };

  // ########## LASER RELATED ##########

  // returns the piece destroyed if the laser is shot
  // null if no piece destroyed
  // does not actually shoot laser -- board representation does not change
  this.laserTarget = function(color) {
    var position = this.findKing(color);
    if (!position) {
      return null;
    }
    var direction = this.getPieceCR(position).orientation;
    var transform = {
      'u': [0, 1],
      'r': [1, 0],
      'd': [0, -1],
      'l': [-1, 0]
    }

    var piece = null;
    while (true) {
      position[0] += transform[direction][0];
      position[1] += transform[direction][1];
      if (position[0] < 0 || position[0] >= NUM_COLS
        || position[1] < 0 || position[1] >= NUM_ROWS) {
        // out of bounds
        return null;
      }
      piece = this.getPieceCR(position);
      if (piece !== null && piece.blocked==false) {
        direction = piece.laserResult(direction);

        if (direction === 's') {
          // Stop
          return null;
        } else if (direction === 'x') {
          // Dead
          return piece;
        }
      }
    }
  };

    this.shootLaser = function() {
	this.pinned = [];
	this.victim = this.laserTarget(this.turn);
	this.drawLaser(LASER_COLORS[0], ZAPPED_PIECE_COLOR, FROZEN_PIECE_COLOR);
	if (this.victim != null) {
	    var hitCol = this.victim.position[0];
	    var hitRow = 1*this.victim.position[1];
	    this.board[hitCol][hitRow] = null;
	    this.victim.remove();
	}
	this.switchPlayer();
    };
    
    this.shootLaserPreview = function() {
	var newBoard = new Board();
	var lineWidth = 2;
	newBoard.setBoardFen(this.getBoardFen());
	var victim = newBoard.laserTarget(this.turn);
	newBoard.drawLaser(LASER_COLORS[0], ZAPPED_PIECE_COLOR, FROZEN_PIECE_COLOR, false);
	if (victim !== null) {
	    lineWidth += 1;
	    var hitCol = victim.position[0];
	    var hitRow = 1*victim.position[1];
	    newBoard.board[hitCol][hitRow] = null;
	}
    };

  this.shootLaserIdle = function() {
    this.drawLaser('#FF0000', ZAPPED_PIECE_COLOR, FROZEN_PIECE_COLOR);
    this.victim = this.laserTarget(this.turn);
    if (this.victim) {
      var hitCol = this.victim.position[0];
      var hitRow = 1*this.victim.position[1];
      this.board[hitCol][hitRow] = null;
    }
    this.turn = (this.turn === 'black') ? 'white' : 'black';
  };

  // ########## DRAWING RELATED ##########

  this.drawBoard = function() {
    $('#board-parent img').remove(); // Clear board
    for (var r = NUM_ROWS-1; r >= 0; r--) {
      for (var c = 0; c < NUM_COLS; c++) {
        var piece = this.getPiece(COL_NAMES[c] + r);
        if (piece) {
          piece.draw();
        }
      }
    }
  };

  this.posToCoord = function(position) {
    return [PIECE_SIZE/2 + position[0] * PIECE_SIZE, PIECE_SIZE/2 + (NUM_ROWS - position[1] - 1) * PIECE_SIZE];
  };

  this.clearLaser = function() {
    if (this.victim) {
      this.victim.remove();
    }
    var laserCanvas = document.getElementById('laserCanvas');
    var ctx = laserCanvas.getContext('2d');
    ctx.clearRect(0, 0, BOARD_WIDTH, BOARD_HEIGHT);
  }

  this.drawLaser = function(strokeColor, fillColor, fillColor2, shouldKeep, lineWidth) {
    var laserCanvas = document.getElementById('laserCanvas');
    var ctx = laserCanvas.getContext('2d');
    var color_2_rgb = fillColor2.split('(')[1].split(')')[0].split(',');
    var color_rgb = fillColor.split('(')[1].split(')')[0].split(',');
    ctx.strokeStyle = strokeColor;
    ctx.lineWidth = lineWidth ? lineWidth : 5;
    ctx.globalCompositeOperation='destination-over';
    // ctx.globalAlpha = 0.8;
    if (!shouldKeep) {
      ctx.clearRect(0, 0, BOARD_WIDTH, BOARD_HEIGHT);
    }

    var position = this.findKing(this.turn);

    var coord = this.posToCoord(position);
    ctx.beginPath();
    ctx.moveTo(coord[0], coord[1]);

    var direction = this.getPieceCR(position).orientation;
    var transform = {
      'u': [0, 1],
      'r': [1, 0],
      'd': [0, -1],
      'l': [-1, 0]
    }

    var piece = null;
    var hitsomething = false;
    var skip_direction_change = false;
    var previous_direction = null;
    var freeze_color = this.turn == "black" ? "white" : "black";
    while (true) {
      if (skip_direction_change) {
        direction = previous_direction;
        skip_direction_change = false;
      } else {
        previous_direction = direction;
      }
      position[0] += transform[direction][0];
      position[1] += transform[direction][1];
      if (position[0] < 0 || position[0] >= NUM_COLS
        || position[1] < 0 || position[1] >= NUM_ROWS) {
        // out of bound
        coord = this.posToCoord(position);
        ctx.lineTo(coord[0], coord[1]);
        break;
      }
      piece = this.getPieceCR(position);
      if (piece !== null) {
        coord = this.posToCoord(position);
        ctx.lineTo(coord[0], coord[1]);
        direction = piece.laserResult(direction);

        if (direction === 's') {
          // Stop
          if (hitsomething) {
            break;
          } else {
            //fillColor = fillColor2;
            hitsomething = true;
            skip_direction_change = true;
          }
        } else if (direction === 'x') {
          if (hitsomething && piece.color == freeze_color) {
            // Frozen
            var rect_x = position[0] * PIECE_SIZE;
            var rect_y = (NUM_ROWS - position[1] - 1) * PIECE_SIZE
            var p = ctx.getImageData(Math.ceil(rect_x), Math.ceil(rect_y), 1, 1).data;
            // If color already set, don't set again
            if (p[0] != color_2_rgb[0] || p[1] != color_2_rgb[1] || p[2] != color_2_rgb[2]) {
              ctx.fillStyle = fillColor2;
              ctx.fillRect(rect_x,
                           rect_y,
                           PIECE_SIZE - 2 * PIECE_MARGIN,
                           PIECE_SIZE - 2 * PIECE_MARGIN);
              this.pinned.push(piece);
            }
            break;
          } else {
            ctx.fillStyle = fillColor;
            ctx.fillRect(position[0] * PIECE_SIZE,
                     (NUM_ROWS - position[1] - 1) * PIECE_SIZE,
                     PIECE_SIZE - 2 * PIECE_MARGIN,
                     PIECE_SIZE - 2 * PIECE_MARGIN);
            skip_direction_change = true;
            hitsomething = true;
            break;
          }
        } else if (piece.color == freeze_color) {
            // Reflecting?
            if (hitsomething) {
              ctx.fillStyle = fillColor2;
            } else {
              ctx.fillStyle = fillColor2;
            }
            var rect_x = position[0] * PIECE_SIZE;
            var rect_y = (NUM_ROWS - position[1] - 1) * PIECE_SIZE
            var p = ctx.getImageData(Math.ceil(rect_x), Math.ceil(rect_y), 1, 1).data;
            // If color already set, don't set again
            if (p[0] != color_2_rgb[0] || p[1] != color_2_rgb[1] || p[2] != color_2_rgb[2]) {
              ctx.fillRect(rect_x,
                           rect_y,
                           PIECE_SIZE - 2 * PIECE_MARGIN,
                           PIECE_SIZE - 2 * PIECE_MARGIN);
              this.pinned.push(piece);
            }
        }
      }
    }
    ctx.stroke();
  };
};

