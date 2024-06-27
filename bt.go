package main

import (
	"machine"
	"time"
	"math/rand"
)

// Motor definitions
var motorL = machine.Pin(1)
var motorR = machine.Pin(4)

// Bluetooth pins
var BT_RXD = machine.ADC{machine.PinA5}
var BT_TXD = machine.ADC{machine.PinA4}

// Ultrasonic sensor pins
var TrigPin = machine.PinA0
var EchoPin = machine.PinA1
var duration int64
var distance float64

// LED pins
var RedL = machine.PinA2
var GreenL = machine.PinA3

// Motor speeds
var Lspeed = 250
var Rspeed = 250

// Mode
var autoMode = false

func setup() {
	// Initialize motors
	motorL.Configure(machine.PinConfig{Mode: machine.PinOutput})
	motorR.Configure(machine.PinConfig{Mode: machine.PinOutput})

	// Initialize pins
	EchoPin.Configure(machine.PinConfig{Mode: machine.PinInput})
	TrigPin.Configure(machine.PinConfig{Mode: machine.PinOutput})
	RedL.Configure(machine.PinConfig{Mode: machine.PinOutput})
	GreenL.Configure(machine.PinConfig{Mode: machine.PinOutput})

	// Initialize serial communication
	machine.UART0.Configure(machine.UARTConfig{
		BaudRate: 9600,
		TX:       BT_TXD.Pin(),
		RX:       BT_RXD.Pin(),
	})
}

func loop() {
	for {
		if machine.UART0.Buffered() > 0 {
			val, _ := machine.UART0.ReadByte()
			switch val {
			case 'f':
				autoMode = false
				GreenL.Low()
				RedL.Low()
				Forward()
			case 'b':
				autoMode = false
				GreenL.Low()
				RedL.Low()
				Backward()
			case 'r':
				autoMode = false
				GreenL.Low()
				RedL.Low()
				Right()
			case 'l':
				autoMode = false
				GreenL.Low()
				RedL.Low()
				Left()
			case 's':
				autoMode = false
				GreenL.Low()
				RedL.Low()
				Stop()
			case 'a':
				autoMode = true
			}
		}

		if autoMode {
			Forward()
			GreenL.High()
			RedL.Low()
			time.Sleep(50 * time.Millisecond)
			Obstacle_Check()
		}
	}
}

func Distance_Measurement() {
	TrigPin.Low()
	time.Sleep(2 * time.Microsecond)
	TrigPin.High()
	time.Sleep(10 * time.Microsecond)
	TrigPin.Low()
	duration = int64(machine.TimePulse(EchoPin, machine.High))
	distance = (float64(duration) / 2) * 0.0343
	time.Sleep(50 * time.Millisecond)
}

func Obstacle_Check() {
	val := rand.Intn(2)
	Distance_Measurement()
	time.Sleep(50 * time.Millisecond)
	for distance < 300 {
		GreenL.Low()
		RedL.High()
		if distance < 150 {
			Backward()
			time.Sleep(800 * time.Millisecond)
			Stop()
			time.Sleep(50 * time.Millisecond)
			Distance_Measurement()
			time.Sleep(100 * time.Millisecond)
		} else {
			if val == 0 {
				Right()
				time.Sleep(400 * time.Millisecond)
			} else if val == 1 {
				Left()
				time.Sleep(400 * time.Millisecond)
			}
			Distance_Measurement()
			time.Sleep(100 * time.Millisecond)
		}
	}
}

func Forward() {
	motorL.High()
	motorR.High()
}

func Backward() {
	motorL.Low()
	motorR.Low()
}

func Right() {
	motorL.High()
	motorR.Low()
}

func Left() {
	motorL.Low()
	motorR.High()
}

func Stop() {
	motorL.Low()
	motorR.Low()
}

func main() {
	setup()
	for {
		loop()
	}
}
