const canvas = document.getElementById('myCanvas');
const ctx = canvas.getContext('2d');
remainderRed = 2;
remainderGreen = 2;

// Initial circle state
let circle = {
  x: 100,
  y: 100,
  radius: 10,
  color: 'black'
};

// Draw circle function
function drawCircle() {
  ctx.clearRect(0, 0, canvas.width, canvas.height);
  ctx.beginPath();
  ctx.arc(circle.x, circle.y, circle.radius, 0, Math.PI * 2);
  ctx.fillStyle = circle.color;
  ctx.fill();
  ctx.closePath();
}

drawCircle();

// Add click listener
canvas.addEventListener('click', function(event) {
  const rect = canvas.getBoundingClientRect();
  const clickX = event.clientX - rect.left;
  const clickY = event.clientY - rect.top;

  const dx = clickX - circle.x;
  const dy = clickY - circle.y;

  if (Math.sqrt(dx * dx + dy * dy) <= circle.radius) {
    circle.color = circle.color === 'red' ? 'green' : 'red';
    drawCircle();
  }
});

