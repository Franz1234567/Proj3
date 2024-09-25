#ifndef OPERATIONAL_H
#define OPERATIONAL_H

#include "state.h"
#include "initialization.h"
#include "idle.h"

class OperationalState : public State
{
public:
  void on_do() override;
  void on_entry() override;
  void on_exit() override;
  void on_event(char event) override;
};

#endif