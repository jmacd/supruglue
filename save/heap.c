// 5120 before (with this)
// 4564 after

void heapUp(int32_t position) {
  while (1) {
    int32_t parent = (position - 1) / 2;

    if (parent == position || schedule.pending.queue[parent].when.NANOS < schedule.pending.queue[position].when.NANOS) {
      break;
    }
    ScheduleItem tmp = schedule.pending.queue[position];
    schedule.pending.queue[position] = schedule.pending.queue[parent];
    schedule.pending.queue[parent] = tmp;
    position = parent;
  }
}

void heapDown(int32_t i, int32_t n) {
  for (;;) {
    int32_t left = 2 * i + 1;
    if (left >= n) {
      return;
    }
    int32_t lesser = left;
    int32_t right = lesser + 1;
    if (right < n && schedule.pending.queue[right].when.NANOS < schedule.pending.queue[left].when.NANOS) {
      lesser = right;
    }
    ScheduleItem ii = schedule.pending.queue[i];
    ScheduleItem li = schedule.pending.queue[lesser];
    if (ii.when.NANOS < li.when.NANOS) {
      return;
    }
    schedule.pending.queue[i] = li;
    schedule.pending.queue[lesser] = ii;
    i = lesser;
  }
}

int Sleep(Duration d) {
  if (schedule.pending.size == sizeof(schedule.pending.queue)) {
    return -1;
  }

  // see container/heap.Push
  ScheduleItem *item = &schedule.pending.queue[schedule.pending.size++];
  item->tptr = __system_current;

  ReadClock(&item->when);
  TimeAdd(&item->when, d);

  heapUp(schedule.pending.size - 1);

  YieldBlocked();

  return 0;
}

void clockProcess(ThreadID thid, Args args) {
  while (1) {
    Clock clk;
    ReadClock(&clk);

    while (clk.NANOS >= schedule.pending.queue[0].when.NANOS) {
      ThreadListPushFront(&__system_runnable, schedule.pending.queue[0].tptr);

      int32_t n = schedule.pending.size - 1;
      schedule.pending.queue[0] = schedule.pending.queue[n];
      heapDown(0, n);
    }

    Yield();
  }
}
