; $Id: README.txt,v 1.1.4.2 2010/11/18 11:05:46 univate Exp $

-----------------------------------------------------------
SMS Framework Message Tracking feature module
-----------------------------------------------------------

** THIS MODULE IS VERY UNFINISHED **

- The module is tracking messages fine, by recording them, marking them with
  a status and updating the status when the gateway sends receipts.
- The Views are not working (I am crap at Views).
- The API fns and hooks have not been written.
- The admin interface is incomplete.
- The purge scheduler may need to be tested and extended.


** LIST OF WANTED FEATURES: **

- Archive in/out/all messages
  - Purge archived messages after a time period
- Track message delivery receipts
  - 0. Place a tracking reference on a sent message
  - 0. (or) Record the message ID given by the gateway?
  - 1. Record message send and gateway response (if exist)
  - 2. Take receipt and modify message status
- Admin interface with View for recorded/tracked messages
  - Has red/yellow/green markers and views filters
  - Delivery stats summary
    - Delivery success/failure
  - Google charts for delivery stats
- API
  - Implement functions and hook that allow others to interact with
    the message tracking system.
- Janitorial
  - Purge delivered messages after a time period
  - Purge non-delivered messages after a time period

