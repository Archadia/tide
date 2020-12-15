#ifndef TIDE_PROCESS_H
#define TIDE_PROCESS_H

namespace tide
{

void ProcessInit();
void ProcessUpdate(double t, double dt);
void ProcessDraw();
void ProcessClose();
   
}
#endif // TIDE_PROCESS_H