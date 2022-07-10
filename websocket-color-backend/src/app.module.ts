import { ColorsModule } from './colors/colors.module';
import { Module } from '@nestjs/common';
import { AppController } from './app.controller';
import { AppService } from './app.service';

@Module({
  imports: [ColorsModule],
  controllers: [AppController],
  providers: [AppService],
})
export class AppModule {}
