import { ColorsGateway } from './colors.gateway';

import { Module } from '@nestjs/common';

@Module({
  providers: [ColorsGateway],
})
export class ColorsModule {}
