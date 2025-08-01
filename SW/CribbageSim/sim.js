const canvas = document.getElementById('myCanvas');
const ctx = canvas.getContext('2d');
ctx.font = "50px Arial";

const circleRadius = 10;
const numberExteriorCircles = 60;
const maxHand = 29;
const circlePosX = 100;
const circlePosY = 300;
remainderRed = 2;

function generateHexPositions(leftCornerX,leftCornerY) { 
  const numberPointsPerLine = 10;
  const distanceBetweenPoints = 40;
  const horizontalDistancebetweenPoints = 30
  let output = [];
  output.push([leftCornerX,leftCornerY])
  let rollingX = leftCornerX;
  let rollingY = leftCornerY;
  // First, 45 degrees up and to the right
  // C^2 = A^2 + B^2, but since 45 degrees, C^2 = 2A^2
  // distanceBetweenPoints^2 / 2 = A^2, A = (distanceBetweenPoints^2 / 2) ^ 0.5
  for (let x = 0; x < numberPointsPerLine; x++) {
    rollingX = Math.round(rollingX + ((distanceBetweenPoints ^ 2) / 2) ^ 0.5);
    rollingY = Math.round(rollingY - ((distanceBetweenPoints ^ 2) / 2) ^ 0.5);
    output.push([rollingX,rollingY]);
  }
  // Now for straight line right
  for (let x = 0; x < numberPointsPerLine; x++) {
    rollingX = rollingX + horizontalDistancebetweenPoints;
    output.push([rollingX,rollingY]);
  }
  // Down and to the right
  for (let x = 0; x < numberPointsPerLine; x++) {
    rollingX = Math.round(rollingX + ((distanceBetweenPoints ^ 2) / 2) ^ 0.5);
    rollingY = Math.round(rollingY + ((distanceBetweenPoints ^ 2) / 2) ^ 0.5);
    output.push([rollingX,rollingY]);
  }
  // Down and to the left
  for (let x = 0; x < numberPointsPerLine; x++) {
    rollingX = Math.round(rollingX - ((distanceBetweenPoints ^ 2) / 2) ^ 0.5);
    rollingY = Math.round(rollingY + ((distanceBetweenPoints ^ 2) / 2) ^ 0.5);
    output.push([rollingX,rollingY]);
  }
  // Straight line left
  for (let x = 0; x < numberPointsPerLine; x++) {
    rollingX = rollingX - horizontalDistancebetweenPoints;
    output.push([rollingX,rollingY]);
  }

  // Back up and to the left.
  for (let x = 0; x < numberPointsPerLine - 1; x++) {
    rollingX = Math.round(rollingX - ((distanceBetweenPoints ^ 2) / 2) ^ 0.5);
    rollingY = Math.round(rollingY - ((distanceBetweenPoints ^ 2) / 2) ^ 0.5);
    output.push([rollingX,rollingY]);
  }

  return output;
}

let exteriorPositions = generateHexPositions(circlePosX,circlePosY);
let exteriorColors = [];
for (let x = 0; x < numberExteriorCircles; x++) {
  exteriorColors[x] = 'black';
}

function getSinglePosition(x) {
  const pixel = (ctx.getImageData(exteriorPositions[x][0], exteriorPositions[x][1], 1, 1)).data;
  return pixel;
}

function isPixelBlack(x) {
  const pixel = getSinglePosition(x);
  return (pixel[0] < 5 && pixel[1] < 5 && pixel[2] < 5);
}

function readPositions() {
  let val = "";
  for (let x = 0; x < numberExteriorCircles; x++) {
    if (isPixelBlack(x)) {
      val = val + "0";
    } else {
      val = val + "1";
    }
  }
  return val;
}

function findFirstBit(value) {
  for (let lastVal = 0; lastVal < 64; lastVal++) { 
    if (value[lastVal] === "1") {
      return lastVal;
    }
  }
  return 0;
}

function findLastBit(value) {
  for (let lastVal = 64; lastVal > 0; lastVal--) {
    if (value[lastVal] === "1") {
      return lastVal;
    }
  }
  return 0;
}

// Draw circle function
function drawCircle(circleX,circleY,color) {
  ctx.beginPath();
  ctx.arc(circleX, circleY, circleRadius, 0, Math.PI * 2);
  ctx.fillStyle = color;
  ctx.fill();
  ctx.closePath();
}

ctx.clearRect(0, 0, canvas.width, canvas.height);
for (let x = 0; x < numberExteriorCircles; x++) {
  drawCircle(exteriorPositions[x][0],exteriorPositions[x][1],exteriorColors[x]);
}

// Add click listener
canvas.addEventListener('click', function(event) {
  const rect = canvas.getBoundingClientRect();
  const clickX = event.clientX - rect.left;
  const clickY = event.clientY - rect.top;

  for (let x = 0; x < numberExteriorCircles; x++) {
    const dx = clickX - exteriorPositions[x][0];
    const dy = clickY - exteriorPositions[x][1];  
    if (Math.sqrt(dx * dx + dy * dy) <= circleRadius) {
      if (exteriorColors[x] === 'black') {
        if (remainderRed > 0) {
          exteriorColors[x] = 'red';
          remainderRed = remainderRed - 1;  
        }
      } else {
        exteriorColors[x] = 'black';
        remainderRed = remainderRed + 1;
      }
      drawCircle(exteriorPositions[x][0],exteriorPositions[x][1],exteriorColors[x]);
      break;
    }
  }

  if (remainderRed == 0) {
    const currentVal = readPositions();
    let first = findFirstBit(currentVal);
    const last = findLastBit(currentVal);
    let difference = last-first;
    if (difference > maxHand) {
      first = first + numberExteriorCircles;
      difference = first - last;
    }

    ctx.fillText(`${difference}`,circlePosX+100,circlePosY);
  } else {
    ctx.clearRect(circlePosX+100, circlePosY-50, 300,100);
  }

});

