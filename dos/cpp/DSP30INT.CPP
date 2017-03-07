BEGINNING_OF_INTERRUPT

  loc_rr3 = read(RR3);
  if ((loc_rr3 & 0x07) == 0) goto Exit;

Loop:
  loc_rr2 = read(RR2);
  loc_rr2 >>= 1;
  if (loc_rr2 & 3) goto TstRxAvail;

TxEmpty:                               //case 0
  if (tx_current < tx_count) goto SendTx;
  write(WR0, 0x28);
  tx_finished = 1;
  goto LookAgain;

SendTx:
  write(WR8, tx_buffer[current_tx]);
  tx_current++;
  goto LookAgain;

TstRxAvail:
  if (loc_rr2 == 2) goto RxAvail;
  if (loc_rr2 == 3) goto RxSpecCond;
  if (loc_rr2 != 1) goto LookAgain;    //should never happen

TxUnderrun:
  write(WR0, 0x10);
  if (!tx_finished) goto TxEmpty;

DoneTxing:
  QueueTask(Reset_RTS_task);
  goto LookAgain;

RxSpecCond:
  loc_rr1 = read(RR1);
  if ((loc_rr1 & 0x20) != 0) goto RxOverrun;
  if ((loc_rr1 & 0x80) == 0) goto RxError;

EndOfFrame:
  if ((loc_rr1 & 0x60) != 0) goto CRCError;

  (handle packet by type)
  goto RxFinished;

RxFinished:
  read(RR8);
  read(RR8);
  read(RR8);
  write(WR0, 0x30);
  write(WR0, 0x10);
  rx_current = 0;
  rx_finished = 1;
  goto LookAgain;

CRCError:
RxOverrun:
RxError:
  write(WR0, 0x30);
  read(RR8);
  read(RR8);
  read(RR8);
  rx_current = 0;
  goto LookAgain;

RxAvail:
  loc_rr1 = read(RR1);
  if ((loc_rr1 & 0x20) != 0) goto RxOverrun;
  rx_buffer[rx_current] = read(RR8);
  rx_current++;
  goto LookAgain;

LookAgain:
  loc_rr3 = read(RR3);
  if (loc_rr3 & 0x07) goto Loop;

END_OF_INTERRUPT
