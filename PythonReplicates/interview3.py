def canAttendAllMeetings(intervals):
    """
    Determines if a person can attend all meetings without any time conflicts.

    Parameters:
    intervals (List[List[int]]): A list of meeting intervals where each interval is a list of two integers [start, end].

    Returns:
    bool: True if the person can attend all meetings without conflicts, False otherwise.
    """

    # Step 1: Sort the intervals by their start time.
    intervals.sort(key=lambda x: x[0])

    # Step 2: Iterate through the sorted intervals and check for any overlaps.
    for i in range(1, len(intervals)):
        # Compare the end time of the previous interval with the start time of the current interval.
        if intervals[i][0] < intervals[i - 1][1]:
            # If there's an overlap, return False.
            return False

    # Step 3: If no overlaps are found, return True.
    return True


intervals = [[7, 10], [2, 4]]


print(canAttendAllMeetings(intervals))  # Output: False
