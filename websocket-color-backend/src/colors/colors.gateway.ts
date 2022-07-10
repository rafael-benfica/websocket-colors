import {
  ConnectedSocket,
  MessageBody,
  SubscribeMessage,
  WebSocketGateway,
  WebSocketServer,
  WsResponse,
} from '@nestjs/websockets';
import { Server, Socket } from 'socket.io';

@WebSocketGateway({
  cors: true,
  namespace: 'colors',
})
export class ColorsGateway {
  static color = '#FFFFFFFF';

  @WebSocketServer()
  server: Server;

  @SubscribeMessage('setColor')
  handleEvent(@MessageBody() color: string, @ConnectedSocket() socket: Socket) {
    if (color) {
      ColorsGateway.color = color;
      console.log(new Date(), ColorsGateway.color);
      socket.broadcast.emit('colorChange', color);
    }
  }

  @SubscribeMessage('getColor')
  getColor(): WsResponse<string> {
    return { event: 'getColor', data: ColorsGateway.color };
  }
}
