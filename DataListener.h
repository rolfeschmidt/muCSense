/*
# copyright 2012 by Rolfe Schmidt

	This file is part of muCSense.

    muCSense is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    muCSense is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with muCSense.  If not, see <http://www.gnu.org/licenses/>.

*/
/*
  DataListener.h
  
  This is the interface for objects that can listen for updates from DataCollector
  instances.  In applications, these will often be Calibrators, listening for filtered data.
  However, other Listeners can be used to report data to an output stream, write to a file, etc.  
  DataListeners are OBservers in the Observer design pattern.
*/

#ifndef __DATA_LISTENER_H
#define __DATA_LISTENER_H


class DataCollector;

class DataListener {
  public:
    virtual void update(const DataCollector* pSubject) = 0;
    
};


#endif // __DATA_LISTENER_H
