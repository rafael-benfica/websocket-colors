import { io } from "socket.io-client";

const socket = io("http://192.168.0.20:3000/colors");

export default socket