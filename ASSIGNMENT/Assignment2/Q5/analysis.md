# Q5 Analysis

Static data belongs to the class rather than an individual object, so only one `issued` counter exists. The out-of-class definition `int BusTicket::issued = 0;` supplies storage.

A static member function has no `this` pointer, so it cannot directly access a non-static member.

The array `trip[4]` creates four objects. The first three receive seats and the fourth is placed on the waiting list.

A static counter, global counter, and separate office object all use O(1) space and O(1) operations. The static-member design has better encapsulation than a global. A separate `TicketOffice` is more flexible when two independent buses must maintain separate capacities and counts.
