const canvas = document.querySelector('canvas')

const c = canvas.getContext('2d')

canvas.width = window.innerWidth
canvas.height = window.innerHeight

//basic notion
//there are three separated componets in game
//1,state variables. Actually the essense of game is just an math. You need to 
//2,renderer. Usually game engine is in charge of rendering. and it jsut renders the state variables.
//3,updater. There are externer factor which changes the state variables to the next state which is so called 'events.
//events are produced usually from keyboards or mouse.
//And that's every thing for creating games. state variables -> rendering -> state update -> rendering acorring to state variables -> state update -> rendering ....


const gravity = 1.5

class Player {
    constructor(){
        this.position = {
            x: 100,
            y: 100
        }
        this.velocity = {
            x:0,
            y:10
        }
        this.width = 30;
        this.height = 30;
    }

    draw() {
        c.fillStyle = 'red'
        c.fillRect(this.position.x,this.position.y
            ,this.width,this.height)
    }

    //this method is in charge of rendering and state update.
    update(){
        
        this.position.y += this.velocity.y
        this.position.x += this.velocity.x
        this.draw()

        if(this.position.y + this.height + this.velocity.y <= canvas.height){
            this.velocity.y += gravity
        }
        else{
            this.velocity.y = 0;
        }
        
    }

}

class Platform {
    constructor(){
        this.position = {
            x:300,
            y:300
        }

        this.width = 300
        this.height = 100
    }

    draw(){
        c.fillStyle = 'blue'
        c.fillRect(this.position.x,this.position.y,
            this.width,this.height)
    }
}

const player = new Player()
const platform = new Platform()

const keys = {
    right:{
        pressed:false,
    },
    left:{
        pressed:false
    }

}

function animate(){
    //this function calls function over and over again
    requestAnimationFrame(animate)
    c.clearRect(0,0,canvas.width,canvas.height)
    player.update()
    platform.draw()

    if(keys.right.pressed){
        player.velocity.x = 5;
    }
    else if(keys.left.pressed){
        player.velocity.x = -5;
    }
    else{
        player.velocity.x = 0;
    }

 
}

//by here was just the definition of functions or someting blabla

//updater in which rendering and state updating is done
animate();


//eventlistenr
addEventListener('keydown',({ keyCode }) => {
    console.log(keyCode)
    switch(keyCode){
        case 65:
            console.log('left')
            keys.left.pressed = true;
            break
        case 87:
            console.log('up')
            player.velocity.y -= 8;
            break;
        case 68:
            console.log('right')
            keys.right.pressed = true;
            break;
        case 83:
            console.log('down');
            break;
        case 13:
            console.log('fire');
            break;
    }
})


//eventlistenr
addEventListener('keyup',({ keyCode }) => {
    console.log(keyCode)
    switch(keyCode){
        case 65:
            console.log('left')
            keys.left.pressed = false;
            break
        case 87:
            console.log('up')
            player.velocity.y -= 20;
            break;
        case 68:
            console.log('right')
            keys.right.pressed = false;
            break;
        case 83:
            console.log('down');
            break;
        case 13:
            console.log('fire');
            break;
    }
})
