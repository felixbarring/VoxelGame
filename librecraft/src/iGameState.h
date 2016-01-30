/*
 * iGameState.h
 *
 *  Created on: Jan 30, 2016
 *      Author: felix
 */

#ifndef SRC_IGAMESTATE_H_
#define SRC_IGAMESTATE_H_


class IGameState {
public:

	virtual ~IGameState() {}

	virtual void update(float timePassed) = 0;




};


#endif /* SRC_IGAMESTATE_H_ */
