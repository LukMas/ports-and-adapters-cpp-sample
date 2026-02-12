//
// Created by luca on 21/01/26.
//

#ifndef GRABSTATION_KIOSK_H
#define GRABSTATION_KIOSK_H

#pragma once
#include <iostream>
#include <optional>

#include <vector>

#include "IStatusListener.h"
#include "ports/ICommandQueue.h"
#include "ports/IArmPort.h"
#include "ports/ILogger.h"
#include "ports/IViewPort.h"
#include "states/States.h"

class IKioskState;

/**
 * The class that handles the logic of the kiosk machine.
 * It checks the content of the CommandQueue, and when a
 * command is found it passes it to the actual state.
 *
 * The states are modeled in a way that each one is able to
 * manage only a given set of commands, avoiding any type of
 * unwanted transaction.
 *
 * The Kiosk class uses two ports:
 * - the IViewPort is used to inform about the internal status
 * of the Kiosk (to say a message or show an error)
 * - the IArmPort is used for the interaction with the Arm
 * when the state requires its movement
 *
 * The registred observers are used to notify any external service
 * about the changes in the states of the Kiosk.
 */
class Kiosk
{
    ICommandQueue& m_queue;
    IViewPort& m_view;
    IArmPort& m_arm;
    ILogger& m_log;
    IKioskState* m_currentState;

    std::vector<IStatusListener*> m_statusListeners;

    int m_rows;
    int m_cols;



    /**
     * The function that notifies the status of the machine
     * to every registered observer.
     * @param s the status of the machine
     */
    void notifyListeners(MachineStatus s) const
    {
        for (auto* l : m_statusListeners)
        {
            l->onStatusChanged(s);
        }
    }

public:
    /**
     * Constructor, it requires the queue for the commands,
     * the view port to output the status and the arm port
     * to move the arm.
     * @param q the instance of the queue
     * @param v the view port
     * @param a the arm port
     * @param log the logger
     * @param rows number of rows of the kiosk, always bigger than 0
     * @param cols number of cols of the kioks, always bigger than 0
     */
    Kiosk(ICommandQueue& q, IViewPort& v, IArmPort& a, ILogger& log, int rows, int cols);

    void addStatusListener(IStatusListener* l)
    {
        m_statusListeners.push_back(l);
    }

    /**
     * Accessor to obtain the view port
     * @return the instance of the view port
     */
    [[nodiscard]] IViewPort& getView() const
    {
        return m_view;
    }

    /**
     * Accessor to obtain the arm port
     * @return the istance of the arm handler
     */
    [[nodiscard]] IArmPort& getArm() const
    {
        return m_arm;
    }

    /**
     * The function handles the first state transition,
     * that initialize the Kiosk and prepares it for
     * listening to the user command.
     * The function should be called once the Kiosk has
     * been completely initialized, just before the
     * creation of the thread that executes the step
     * function.
     */
    void start() const
    {
        this->notifyListeners(MachineStatus::INITIALIZING);
        this->getView().notifyMessage("System Powering On...");

        // once everything is ok I send the READY command to
        // move to the IDLE state
        this->m_queue.push(KioskCommand(CommandType::READY));
    }

    /**
     * The function that is called at every cycle. It takes the command from the command queue and pass it to the
     * state machine, that processes it and returns the next state.
     */
    void step();


    /**
     * The function validates the given coordinates against the size of the grid where the kiosk operates. If the
     * coordinates are valid they will be
     * @param currentCoordinates a coordinate object to verify agains the grid
     * @return an optional that contains the coordinates, if valid, or nothing, if not valid
     */
    [[nodiscard]] std::optional<Coordinate> validateCoordinates(Coordinate currentCoordinates) const
    {
        // checking against grid size
        if (currentCoordinates.x > m_cols || currentCoordinates.y > m_rows)
        {
            std::cout << "Too big coordinates. " << std::endl;
            std::cout << "Actual size: " << m_cols << " and " << m_rows << ". " << std::endl;
            std::cout << "Given size: " << currentCoordinates.x << " and " << currentCoordinates.y << "." << std::endl;

            return std::nullopt;
        }

        return currentCoordinates;
    }

    ~Kiosk();
};

#endif
